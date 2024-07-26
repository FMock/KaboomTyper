#include <SDL.h>
#include "GameManager.h"
#include "Utilities.h"
#include "TextBlock.h"
#include "GlobalPreprocessorFlags.h"
#include <iostream>
#include <functional>
#include "Common.h"
#include "GameStates.h"

#define DEBUG_GAMEMANAGER 1

using namespace GameEngine;
using namespace GameEngine::Utility;
using namespace KaboomTyperDB;

// Loads all the parts of the game
void GameManager::Initialize()
{
    m_exitGame = false;

    // Word Manager
    m_wordManager = std::make_shared<WordManager>();

    // User Input
    m_inputManager = std::make_shared<InputManager>();
	m_inputManager->RegisterObserver(shared_from_this());

    // State Machine
	m_stateMachine = std::make_unique<StateMachine>();

    // UI Manager
    m_uiManager = std::make_unique<UIManager>(m_inputManager, m_wordManager);
    m_uiManager->AddCallback(std::bind(&GameManager::UserScored, this));
    m_uiManager->AddGameOverCallback(std::bind(&GameManager::GameOver, this));
    m_uiManager->AddStartGameCallback(std::bind(&GameManager::StartGame, this));
    //m_uiManager->AddAudioCallback(std::bind(&GameManager::SetPlayMusic, this, std::placeholders::_1)); // need placeholder since SetPlayMusic takes a bool parameter
    m_uiManager->AddAudioCallback([this](bool playMusic) { this->SetPlayMusic(playMusic); }); // Will using lambdas improve performance?
    m_uiManager->AddWordSpeedCallback([this](const std::string speed) { this->ChangeTextBlockFallSpeed(speed); });

    // TextBlock Manager
    m_textblockManager = std::make_shared<TextBlockManager>(10.0f, m_inputManager, m_wordManager);

    // Firework Explosion Manager
    m_fireworkExplosionManager = std::make_shared<FireworkExplosionManager>();
    m_fireworkExplosionManager->SetShouldFireworkExplode(false);

    // Rectangle of Rectangles
    m_rectangleOfRectangles = std::make_shared<DecorativeRectangle>();
    m_rectangleOfRectangles->Initialize(13, 45, 430, 100, Colors::DARK_GRAY);
    m_rectangleOfRectangles->SetAnimate(false);
    m_rectangleOfRectangles->SetAnimateClockwise(false);
    m_rectangleOfRectangles->SetAnimateRandom(true);

    // Audio
    { // TODO PUT IN AUDIO MANAGER
        m_explosion = std::make_shared<GameAudio>();

        if (!m_explosion->Initialize())
        {
            std::cerr << "Failed to initialize audio!" << std::endl;
        }

        m_explosionPath = Utilities::ReadConfigFileGetPath(Common::GAME_CONFIG_FILE, "explosionWavFilePath");
        if (m_explosionPath.empty())
        {
            std::cout << "Target not found or error reading the config file." << std::endl;
            // TODO: HANDLE THIS ERROR
        }

        m_explosion->LoadWAV("explosion_stereo", m_explosionPath);
    }

    RegisterDrawables();
}

GameManager::GameManager() : m_exitGame(false)
{
	// ctor is private. Must use factory method Create() to get a GameManager
}

std::shared_ptr<GameManager> GameManager::Create()
{
    std::shared_ptr<GameManager> instance(new GameManager());
    instance->Initialize();
    return instance;
}

void GameManager::Update(float dt)
{
    UpdateGameEntities(dt);

    if (m_fireworkExplosionManager->IsFireworkActive())
    {
        m_fireworkExplosionManager->Update(dt);
    }

    if(m_fireworkExplosionManager->GetShouldFireworkExplode())
    {
        m_fireworkExplosionManager->SetIsFireworkActive(true);
        m_fireworkExplosionManager->GenerateFireworkParticles();
        m_fireworkExplosionManager->SetShouldFireworkExplode(false);
    }

    m_exitGame = m_inputManager->ShouldQuit(); // user wishes to exit?
}

// User successfully typed target word/s, Destroy TextBlock and increase score
void GameManager::UserScored()
{
    m_textblockManager->DestroyActiveTextBlock();
    m_uiManager->IncreaseScore();
    m_fireworkExplosionManager->SetShouldFireworkExplode(true);
    m_fireworkExplosionManager->ProcessInput();
    m_explosion->PlaySound("explosion_stereo");
    SDL_Delay(10); // slight pause to hear the sound
}

void GameManager::Render()
{
    m_drawOrderManager.RenderAll();
}

void GameEngine::GameManager::UpdateGameEntities(float deltaTime)
{
    m_inputManager->Update();

    m_textblockManager->Update(deltaTime);

    if (m_textblockManager->TextBlockLimitReached()) // Check if game over state reached
    {
        GameOver();
    }

    m_uiManager->Update(deltaTime);
    m_rectangleOfRectangles->Update(deltaTime);
}

void GameEngine::GameManager::StartGame()
{
    GameState currentState = m_stateMachine->GetCurrentState();
    Common::CurrentState = currentState;
    bool textblockGeneratorRunning = m_textblockManager->IsRunning();

    if (currentState == GameState::IDLE )
    {
        m_stateMachine->TransitionTo(GameState::RUNNING);
        m_uiManager->ChangeMessageBoxMessage("F2: PAUSE", "F3: END GAME");
        m_uiManager->ChangeStartMenuItemLabel("STOP");
        m_rectangleOfRectangles->SetAnimate(false);
        m_rectangleOfRectangles->SetAnimateRandom(false);

        if (!m_textblockManager->IsRunning())
        {
            m_textblockManager->ToggleRunning();
            m_textblockManager->GenerateTextBlock();
        }
    }
    else if (currentState == GameState::PAUSED) // Start game if paused
    {
        m_stateMachine->TransitionTo(GameState::RUNNING);
        m_uiManager->ChangeMessageBoxMessage("F1: PAUSE", "F3: END GAME");

        if (!m_textblockManager->IsRunning())
        {
            m_textblockManager->ToggleRunning();
        }
    }
    else if (currentState == GameState::RUNNING)
    {
        m_rectangleOfRectangles->SetAnimateClockwise(true);
    }
    else if (currentState == GameState::STOPPED)
    {
        m_stateMachine->TransitionTo(GameState::IDLE);
        m_textblockManager->ClearBlockDeque();
        m_textblockManager->SetTextBlockLimitReached(false);
        m_uiManager->ResetScore();

        // Now it's safe to transition to GameState::RUNNING
        m_stateMachine->TransitionTo(GameState::RUNNING);
        m_uiManager->ChangeMessageBoxMessage("F2: PAUSE", "F3: END GAME");
        m_uiManager->ChangeStartMenuItemLabel("STOP");
        m_rectangleOfRectangles->SetAnimate(false);
        m_rectangleOfRectangles->SetAnimateRandom(false);

        if (!m_textblockManager->IsRunning())
        {
            m_textblockManager->ToggleRunning();
            m_textblockManager->GenerateTextBlock();
        }
    }
}

void GameEngine::GameManager::GameOver()
{
    GameState currentState = m_stateMachine->GetCurrentState();
    Common::CurrentState = currentState;
    bool textblockGeneratorRunning = m_textblockManager->IsRunning();

    m_rectangleOfRectangles->SetAnimateClockwise(false);
    m_uiManager->ChangeStartMenuItemLabel("START");

    m_stateMachine->TransitionTo(GameState::STOPPED);
    m_uiManager->GameOver();

    m_rectangleOfRectangles->SetAnimateClockwise(false);
    m_rectangleOfRectangles->SetAnimateRandom(true);

    if (m_textblockManager->IsRunning())
    {
        m_textblockManager->ToggleRunning();
    }
}

void GameManager::RegisterDrawables()
{
    // Firework Manager
    m_fireworkExplosionManager->SetPriority(1);
    m_drawOrderManager.AddDrawable(m_fireworkExplosionManager);

    // Textblock Manager
    m_textblockManager->SetPriority(1);
    m_drawOrderManager.AddDrawable(m_textblockManager);

    // Rectangle of Rectangles
    m_rectangleOfRectangles->SetPriority(8);
    m_drawOrderManager.AddDrawable(m_rectangleOfRectangles);

    m_uiManager->UIManager::RegisterDrawables(m_drawOrderManager);
}


bool GameEngine::GameManager::GetExitGame() const
{
    return m_exitGame;
}

void GameEngine::GameManager::SetExitGame(bool exitGame)
{  
    m_exitGame = exitGame;
}

bool GameEngine::GameManager::GetPlayMusic() const
{
    return m_playMusic;
}

void GameEngine::GameManager::SetPlayMusic(bool playMusic)
{
    m_playMusic = playMusic;

#if DEBUG_GAMEMANAGER
    std::cout << "Play Music is " << m_playMusic << std::endl;
#endif 

}

void GameEngine::GameManager::ChangeTextBlockFallSpeed(const std::string& speed)
{
    std::cout << "Change TextBlock Fall Speed to " << speed << std::endl;
}


void GameManager::RespondToObserved(InputManager* InputMgr)
{
    GameState currentState = m_stateMachine->GetCurrentState();
    Common::CurrentState = currentState;
    bool textblockGeneratorRunning = m_textblockManager->IsRunning();

    if (InputMgr->m_kbState[SDL_SCANCODE_F1])
    {
        StartGame();
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F2])
    {
        m_rectangleOfRectangles->SetAnimateClockwise(false);
        if (currentState == GameState::RUNNING)
        {
            m_stateMachine->TransitionTo(GameState::PAUSED);
            m_uiManager->ChangeMessageBoxMessage("F1: UNPAUSE", "F3: END GAME");

            if (m_textblockManager->IsRunning())
                m_textblockManager->ToggleRunning();
        }
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F3])
    {
        if (currentState == GameState::PAUSED || currentState == GameState::RUNNING)
        {
            GameOver();
        }
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_ESCAPE]) // Check for ESC key press
    {
        m_inputManager->SetShouldQuit(true); // Set the exit flag to true
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F4])
    {

    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F5])
    {

    }
    else
    {
        //std::cout << m_stateMachine->GetCurrentStateAsString() << std::endl;
    }
}
