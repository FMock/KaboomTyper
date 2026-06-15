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
    m_uiManager->AddWordSubmittedCallback([this](const std::string& word) { return this->UserTyped(word); });
    m_uiManager->AddKaboomCallback(std::bind(&GameManager::BlowUpActiveBlock, this)); // "kaboom" clears the block for free
    m_uiManager->AddMenuSelectCallback(std::bind(&GameManager::PlayMenuSelectSound, this)); // click sound on menu selections
    m_uiManager->AddScoreDingCallback(std::bind(&GameManager::PlayScoreDing, this)); // bell ding per scored point
    m_uiManager->AddGameOverCallback(std::bind(&GameManager::GameOver, this));
    m_uiManager->AddStartGameCallback(std::bind(&GameManager::StartGame, this));
    //m_uiManager->AddAudioCallback(std::bind(&GameManager::SetPlayMusic, this, std::placeholders::_1)); // need placeholder since SetPlayMusic takes a bool parameter
    m_uiManager->AddAudioCallback([this](bool playMusic) { this->SetPlayMusic(playMusic); }); // Will using lambdas improve performance?
    m_uiManager->AddWordSpeedCallback([this](const std::string speed) { this->ChangeTextBlockFallSpeed(speed); });
    m_uiManager->AddDifficultyCallback([this](const std::string& difficulty) { this->SetDifficulty(difficulty); });

    // TextBlock Manager
    m_textblockManager = std::make_shared<TextBlockManager>(10.0f, m_inputManager, m_wordManager);

    // Establish the default difficulty (Normal) on the TextBlockManager and the display.
    SetDifficulty(DifficultyName(m_difficulty));

    // Firework Explosion Manager
    m_fireworkExplosionManager = std::make_shared<FireworkExplosionManager>();

    // Rectangle of Rectangles
    m_rectangleOfRectangles = std::make_shared<DecorativeRectangle>();
    m_rectangleOfRectangles->Initialize(13, 45, 530, 100, Colors::DARK_GRAY); // widened to meet the scoreboard; more squares auto-generated from the width
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

        // Load the boom under several keys so overlapping/rapid explosions don't cut each
        // other off (the SDL mixer mixes different keys; the same key would just restart).
        for (int v = 0; v < BOOM_VOICES; ++v)
            m_explosion->LoadWAV("boom_" + std::to_string(v), m_explosionPath);

        // UI click sound played when the user makes a menu selection.
        std::string optionSelectPath = Utilities::ReadConfigFileGetPath(Common::GAME_CONFIG_FILE, "optionSelectWavFilePath");
        if (optionSelectPath.empty())
            std::cout << "optionSelectWavFilePath not found in config file." << std::endl;
        else
            m_explosion->LoadWAV("option_select", optionSelectPath);

        // Bell ding played once per scored point. Loaded under several keys so the rapid
        // cascade of dings (one per point) overlaps instead of cutting itself off.
        std::string scorePointPath = Utilities::ReadConfigFileGetPath(Common::GAME_CONFIG_FILE, "scorePointWavFilePath");
        if (scorePointPath.empty())
            std::cout << "scorePointWavFilePath not found in config file." << std::endl;
        else
            for (int v = 0; v < DING_VOICES; ++v)
                m_explosion->LoadWAV("ding_" + std::to_string(v), scorePointPath);
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

    m_fireworkExplosionManager->Update(dt); // advance any active explosions (cheap when none)

    m_exitGame = m_inputManager->ShouldQuit(); // user wishes to exit?
}

// Fire the explosion sprites and play the boom for a block that was just destroyed.
// Awards NO points — shared by UserTyped() (which adds the score) and BlowUpActiveBlock().
void GameManager::PlayExplosionEffects()
{
    int blasts = m_fireworkExplosionManager->Trigger(); // bright sprite explosions (multiple for long words)

    // Play the boom on the next voice in the pool (round-robin) so consecutive scores
    // don't cut each other off. A couple of long-word blasts layer for a bigger roar.
    int voices = (blasts >= 4) ? 2 : 1;
    for (int v = 0; v < voices; ++v)
    {
        m_explosion->PlaySound("boom_" + std::to_string(m_boomVoice));
        m_boomVoice = (m_boomVoice + 1) % BOOM_VOICES;
    }
}

// Detonate the active TextBlock with no points — used by the "kaboom" cheat word.
void GameManager::BlowUpActiveBlock()
{
    m_textblockManager->DestroyActiveTextBlock();
    PlayExplosionEffects();
}

// User submitted a word: destroy the matching falling block (if any) and award the score.
bool GameManager::UserTyped(const std::string& word)
{
    if (!m_textblockManager->DestroyMatchingTextBlock(word))
        return false;

    PlayExplosionEffects();
    m_uiManager->IncreaseScore();
    return true;
}

// Plays the menu-selection click sound (wired to UIManager via AddMenuSelectCallback).
void GameManager::PlayMenuSelectSound()
{
    m_explosion->PlaySound("option_select");
}

// Plays one bell ding on the next voice in the pool (round-robin) so a rapid cascade of
// score dings layers instead of cutting off. Wired to UIManager via AddScoreDingCallback.
void GameManager::PlayScoreDing()
{
    m_explosion->PlaySound("ding_" + std::to_string(m_dingVoice));
    m_dingVoice = (m_dingVoice + 1) % DING_VOICES;
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
    // Firework Manager - above the text blocks (1) so blasts render over them
    m_fireworkExplosionManager->SetPriority(7);
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

// Changes TextBlock fall speed by adjusting the gravity variable
void GameEngine::GameManager::ChangeTextBlockFallSpeed(const std::string& speed)
{
    std::cout << "Change TextBlock Fall Speed to " << speed << std::endl;

    auto it = Common::GRAVITY_CHOICES.find(speed);

    if (it != Common::GRAVITY_CHOICES.end())
    {
        Common::GRAVITY = it->second;
    }
    else
    {
        throw std::out_of_range("Key not found in GRAVITY_CHOICES map: " + speed);
    }
}

// Set the game difficulty: update the TextBlockManager's spawn behavior and the on-screen label.
void GameEngine::GameManager::SetDifficulty(const std::string& difficulty)
{
    m_difficulty = ParseDifficulty(difficulty);
    m_textblockManager->SetDifficulty(m_difficulty);
    m_uiManager->SetDifficultyDisplay("Difficulty: " + DifficultyName(m_difficulty));

#if DEBUG_GAMEMANAGER
    std::cout << "Difficulty set to " << DifficultyName(m_difficulty) << std::endl;
#endif
}

void GameManager::RespondToObserved(InputManager* InputMgr)
{
    GameState currentState = m_stateMachine->GetCurrentState();
    Common::CurrentState = currentState;
    bool textblockGeneratorRunning = m_textblockManager->IsRunning();

    // Let the menu system handle its keyboard shortcuts / navigation / click-outside first.
    // If it consumed the input this frame, don't run the game-level key handling below
    // (notably ESC, which closes an open menu instead of quitting when a menu is open).
    if (m_uiManager->HandleMenuInput(InputMgr))
        return;

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
    else if (InputMgr->m_kbState[SDL_SCANCODE_ESCAPE] && !InputMgr->m_kbPrevState[SDL_SCANCODE_ESCAPE]) // ESC just pressed (edge)
    {
        // ESC is fully handled by UIManager::HandleMenuInput (above): it closes any open
        // menu/popup, or shows the exit-confirmation dialog when nothing else is open.
        // HandleMenuInput always consumes ESC now, so this branch is effectively unreachable
        // and intentionally does nothing (quitting happens only via the confirmation dialog).
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F4])
    {
        // Do nothing for now
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F5])
    {
        // Do Nothing for now
    }
    else
    {
        //std::cout << m_stateMachine->GetCurrentStateAsString() << std::endl;
    }
}
