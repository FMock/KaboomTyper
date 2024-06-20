#include "GameManager.h"
#include "Utilities.h"
#include "LevelSplashScreen.h"
#include "LevelMainMenu.h"
#include "LevelGamePlay.h"
#include "LevelGameOver.h"
#include "TextBlock.h"
#include "GlobalPreprocessorFlags.h"
#include <iostream>
#include <functional>

using namespace GameEngine;
using namespace GameEngine::Utility;
using namespace KaboomTyperDB;

// Loads all the parts of the game
void GameManager::Initialize()
{
	m_inputTextBox = std::make_shared<InputTextBox>();
	m_inputTextBox->InitializeTextBox(10, 916, 780, 34, Colors::DEFAULT_COLOR, true);
    m_inputTextBox->AddCallback(std::bind(&GameManager::ProcessInput, this)); // Bind ProcessInput() for use as a Callback by InputTextBox

    m_inputManager = std::make_shared<InputManager>();
	m_inputManager->RegisterObserver(m_inputTextBox); // so InputTextbox can respond to user key presses

	m_gameMenu = std::make_shared<Menu>();
	m_inputManager->RegisterObserver(m_gameMenu); // so menu can respond to mouse clicks

	m_inputManager->RegisterObserver(shared_from_this());

	m_messageBox = std::make_unique<MessageBox>();

	m_stateMachine = std::make_unique<StateMachine>();
	m_headsUpDisplay = std::make_unique<HeadsUpDisplay>();
	m_headsUpDisplay->Initialize(445, 43);

    m_textblockManager = std::make_unique < TextBlockManager>(10.0f, m_inputManager);

    m_gamePlayArea.Initialize();

    m_colorPtr = std::make_unique<Color>();
    m_fireworkColorTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap["green"];
    m_firework = std::make_unique<Firework>(m_fireworkColorTexture, 400, 400, 1, 1, 300);

    m_blowUpTextBlock = false;

    m_rectangleOfRectangles = std::make_unique<DecorativeRectangle>();
    m_rectangleOfRectangles->Initialize(13, 45, 430, 100, Colors::DARK_GRAY);
    m_rectangleOfRectangles->SetAnimate(false);
    m_rectangleOfRectangles->SetAnimateClockwise(false);
    m_rectangleOfRectangles->SetAnimateRandom(true);

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
}

GameEngine::GameManager::GameManager()
{
	//Initialize();
}

std::shared_ptr<GameManager> GameManager::Create()
{
    std::shared_ptr<GameManager> instance(new GameManager());
    instance->Initialize();
    return instance;
}

GameEngine::GameManager::~GameManager()
{
	// Delete LevelManager objects from the map
	for (auto& pair : m_levels)
	{
		delete pair.second;
	}

	// Clear the map after deleting the objects
	m_levels.clear();
}

void GameManager::Update(float dt)
{
    UpdateGameEntities(dt);

    if (m_firework->IsActive())
    {
        m_firework->Update(dt);
    }
}

void GameManager::ProcessInput()
{
    // compare input and active text
    std::string activeStr = Common::GetActiveText();
    std::string submittedStr = Common::GetSubmittedText();

    if (activeStr == submittedStr)
    {
        m_textblockManager->DestroyActiveTextBlock();

        // Increase Players Score
        m_headsUpDisplay->IncreaseScore(10); // TODO: HARD CODED AS 10. SCORE INCREASE SHOULD BE BASED ON THE SIZE OF THE TEXT
        m_headsUpDisplay->SetUpdateRequired(true); // HUD needs to be updated

        m_blowUpTextBlock = true;

        // Set Firework color and positon
        m_firework->SetColor(Common::s_previousColor);
        int x = (int)Common::s_currentPosition.first;
        int y = (int)Common::s_currentPosition.second;
        int adjustedX = x + (Common::s_currentTextBlockWidth / 2);
        int halfFontHeight = 16;
        int adjustedY = y + halfFontHeight;
        m_firework->SetPosition(adjustedX, adjustedY);
        m_explosion->PlaySound("explosion_stereo");
        SDL_Delay(10); // slight pause to hear the sound

#if DEBUG
        std::cout << "The strings are equal." << std::endl;
#endif
    }
    else
    {
#if DEBUG
        std::cout << "The strings are not equal." << std::endl;
#endif
    }
}

void GameManager::Render()
{
    // Render first to not hide TextBlocks
    m_gamePlayArea.Render();

    if (m_blowUpTextBlock)
    {
        m_firework->SetIsActive(true);
        m_firework->GenerateParticles();
        m_blowUpTextBlock = false;
    }

    if (m_firework->IsActive())
    {
        m_firework->Draw();
    }

    m_textblockManager->Draw();
	m_headsUpDisplay->Draw();
	m_inputTextBox->Draw();
	m_gameMenu->Draw();
	m_messageBox->Draw();

    m_rectangleOfRectangles->DrawRectangleWithRectangles();
}

void GameEngine::GameManager::UpdateGameEntities(float deltaTime)
{
    m_inputManager->Update();
    m_inputTextBox->Update(deltaTime);

    m_textblockManager->Update(deltaTime);

    if (m_textblockManager->TextBlockLimitReached()) // Check if game over state reached
    {
        GameOver();
    }

    if(m_headsUpDisplay->UpdateRequired()) // only update the HUD if needed
        m_headsUpDisplay->Update();

    m_rectangleOfRectangles->Update(deltaTime);
}

void GameEngine::GameManager::GameOver()
{
    m_stateMachine->TransitionTo(GameState::STOPPED);
    m_messageBox->ChangeMessage("GAME OVER", "F1:  NEW GAME", "ESC: EXIT GAME");
    m_rectangleOfRectangles->SetAnimateClockwise(false);
    m_rectangleOfRectangles->SetAnimateRandom(true);
}

bool GameEngine::GameManager::ShouldQuit()
{
	return m_inputManager->ShouldQuit();
}

void GameManager::RespondToObserved(InputManager* InputMgr)
{
    GameState currentState = m_stateMachine->GetCurrentState();
    bool textblockGeneratorRunning = m_textblockManager->IsRunning();

    if (InputMgr->m_kbState[SDL_SCANCODE_F1])
    {
        if (currentState == GameState::IDLE || currentState == GameState::PAUSED)
        {
            m_stateMachine->TransitionTo(GameState::RUNNING);
            m_messageBox->ChangeMessage("F2: PAUSE", "F3: END GAME");
            m_rectangleOfRectangles->SetAnimate(false);
            m_rectangleOfRectangles->SetAnimateRandom(false);

            if(!textblockGeneratorRunning)
            {
                m_textblockManager->ToggleRunning();
                m_textblockManager->GenerateTextBlock();
            }
        }
        else if (currentState == GameState::PAUSED) // Start game if paused
        {
            m_stateMachine->TransitionTo(GameState::RUNNING);
            m_messageBox->ChangeMessage("F1: PAUSE", "F3: END GAME");

            if (!textblockGeneratorRunning)
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
            m_rectangleOfRectangles->SetAnimate(true);
            m_textblockManager->ClearBlockDeque();
            m_textblockManager->SetTextBlockLimitReached(false);
            m_headsUpDisplay->ResetScore();
        }
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F2])
    {
        m_rectangleOfRectangles->SetAnimateClockwise(false);
        if (currentState == GameState::RUNNING)
        {
            m_stateMachine->TransitionTo(GameState::PAUSED);
            m_messageBox->ChangeMessage("F1: UNPAUSE", "F3: END GAME");

            if (m_textblockManager->IsRunning())
                m_textblockManager->ToggleRunning();
        }
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F3])
    {
        m_rectangleOfRectangles->SetAnimateClockwise(false);
        if (currentState == GameState::PAUSED || currentState == GameState::RUNNING)
        {
            GameOver();

            if (textblockGeneratorRunning)
            {
                m_textblockManager->ToggleRunning();
            }
        }
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F4])
    {

    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F5])
    {

    }
}
