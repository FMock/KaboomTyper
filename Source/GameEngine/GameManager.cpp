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

    m_leftSideBar.Initialize(0, 42, 10, 908, Colors::BLUE, true);
    m_rightSideBar.Initialize(790, 42, 10, 908, Colors::BLUE, true);
    m_topSideBar.Initialize(0, 146, 800, 5, Colors::BLUE, true);
    m_bottomSideBar.Initialize(0, 910, 800, 5, Colors::BLUE, true);

    m_colorPtr = std::make_unique<Color>();
    m_fireworkColorTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap["green"];
    m_firework = std::make_unique<Firework>(m_fireworkColorTexture, 400, 400, 1, 1, 300);

    m_blowUpTextBlock = false;

    m_rectangleOfRectangles = std::make_unique<DecorativeRectangle>();
    m_rectangleOfRectangles->Initialize(13, 45, 430, 100, Colors::DARK_GRAY);
    m_rectangleOfRectangles->SetAnimate(true);
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
    m_rectangleOfRectangles->Update(dt);
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
    m_leftSideBar.Draw();
    m_rightSideBar.Draw();
    m_topSideBar.Draw();
    m_bottomSideBar.Draw();
    m_rectangleOfRectangles->DrawRectangleWithRectangles();
}

void GameEngine::GameManager::UpdateGameEntities(float deltaTime)
{
    m_inputManager->Update();

    m_textblockManager->Update(deltaTime);

    if (m_textblockManager->TextBlockLimitReached()) // Check if game over state reached
    {
        m_stateMachine->TransitionTo(GameState::STOPPED);
        m_messageBox->ChangeMessage(m_stateMachine->GetCurrentStateAsString());
    }

    if(m_headsUpDisplay->UpdateRequired()) // only update the HUD if needed
        m_headsUpDisplay->Update();
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
            m_messageBox->ChangeMessage(m_stateMachine->GetCurrentStateAsString());
            m_rectangleOfRectangles->SetAnimate(false);

            if(!textblockGeneratorRunning)
            {
                m_textblockManager->ToggleRunning();
                m_textblockManager->GenerateTextBlock();
            }
        }
        else if (currentState == GameState::PAUSED) // Start game if paused
        {
            m_stateMachine->TransitionTo(GameState::RUNNING);
            m_messageBox->ChangeMessage(m_stateMachine->GetCurrentStateAsString());

            if (!textblockGeneratorRunning)
            {
                m_textblockManager->ToggleRunning();
            }
        }
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F2])
    {
        if (currentState == GameState::RUNNING)
        {
            m_stateMachine->TransitionTo(GameState::PAUSED);
            m_messageBox->ChangeMessage(m_stateMachine->GetCurrentStateAsString());

            if (m_textblockManager->IsRunning())
                m_textblockManager->ToggleRunning();
        }
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F3])
    {
        if (currentState == GameState::PAUSED || currentState == GameState::RUNNING)
        {
            m_stateMachine->TransitionTo(GameState::STOPPED);
            m_messageBox->ChangeMessage(m_stateMachine->GetCurrentStateAsString());

            if (textblockGeneratorRunning)
            {
                m_textblockManager->ToggleRunning();
            }
        }
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F4])
    {
        if (currentState == GameState::STOPPED)
        {
            m_stateMachine->TransitionTo(GameState::IDLE);
            m_messageBox->ChangeMessage(m_stateMachine->GetCurrentStateAsString());
        }
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F5])
    {
        if (currentState == GameState::IDLE)
        {
            m_messageBox->ChangeMessage("Press F1 to Start");
            m_rectangleOfRectangles->SetAnimate(true);
            m_textblockManager->ClearBlockDeque();
            m_textblockManager->SetTextBlockLimitReached(false);
            m_headsUpDisplay->ResetScore();
        }
    }
}
