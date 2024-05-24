#include "GameManager.h"
#include "Utilities.h"
#include "LevelSplashScreen.h"
#include "LevelMainMenu.h"
#include "LevelGamePlay.h"
#include "LevelGameOver.h"
#include "TextBlock.h"
#include <iostream>
#include <functional> // Add this include

using namespace GameEngine;
using namespace GameEngine::Utility;

// Loads all the parts of the game
void GameManager::Initialize()
{
	m_inputTextBox = std::make_unique<InputTextBox>();
	m_inputTextBox->InitializeTextBox(10, 912, 780, 34, Colors::DEFAULT_COLOR, true);
    m_inputTextBox->AddCallback(std::bind(&GameManager::ProcessInput, this)); // Bind ProcessInput() for use as a Callback by InputTextBox

	//m_inputManager = std::make_unique<InputManager>();
    m_inputManager = std::make_shared<InputManager>();
	m_inputManager->RegisterObserver(m_inputTextBox.get()); // so InputTextbox can respond to user key presses

	m_gameMenu = std::make_unique<Menu>();
	m_inputManager->RegisterObserver(m_gameMenu.get()); // so menu can respond to mouse clicks

	m_inputManager->RegisterObserver(this);

	m_messageBox = std::make_unique<MessageBox>();

	m_stateMachine = std::make_unique<StateMachine>();
	m_headsUpDisplay = std::make_unique<HeadsUpDisplay>();
	m_headsUpDisplay->Initialize(450, 45);

    m_textblockGenerator = std::make_unique < TextBlockGenerator>(10.0f, m_inputManager);
}

GameEngine::GameManager::GameManager()
{
	Initialize();
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
}

void GameManager::ProcessInput()
{
    // compare input and active text
    std::string activeStr = Common::GetActiveText();
    std::string submittedStr = Common::GetSubmittedText();

    if (activeStr == submittedStr)
    {
        std::cout << "The strings are equal." << std::endl;
    }
    else
    {
        std::cout << "The strings are not equal." << std::endl;
    }
}

void GameManager::Render()
{
    m_textblockGenerator->Draw();
	m_headsUpDisplay->Draw();
	m_inputTextBox->Draw();
	m_gameMenu->Draw();
	m_messageBox->Draw();
}

void GameEngine::GameManager::UpdateGameEntities(float deltaTime)
{
    m_inputManager->Update();

    m_textblockGenerator->Update(deltaTime);

    if (m_textblockGenerator->TextBlockLimitReached()) // Check if game over state reached
    {
        m_stateMachine->TransitionTo(GameState::STOPPED);
        m_messageBox->ChangeMessage(m_stateMachine->GetCurrentStateAsString());
    }
}

bool GameEngine::GameManager::ShouldQuit()
{
	return m_inputManager->ShouldQuit();
}

void GameManager::RespondToObserved(InputManager* InputMgr)
{
    GameState currentState = m_stateMachine->GetCurrentState();
    bool textblockGeneratorRunning = m_textblockGenerator->IsRunning();

    if (InputMgr->m_kbState[SDL_SCANCODE_F1])
    {
        if (currentState == GameState::IDLE || currentState == GameState::PAUSED)
        {
            m_stateMachine->TransitionTo(GameState::RUNNING);
            m_messageBox->ChangeMessage(m_stateMachine->GetCurrentStateAsString());

            if(!textblockGeneratorRunning)
            {
                m_textblockGenerator->ToggleRunning();
                m_textblockGenerator->GenerateTextBlock();
            }
        }
        else if (currentState == GameState::PAUSED) // Start game if paused
        {
            m_stateMachine->TransitionTo(GameState::RUNNING);
            m_messageBox->ChangeMessage(m_stateMachine->GetCurrentStateAsString());

            if (!textblockGeneratorRunning)
            {
                m_textblockGenerator->ToggleRunning();
            }
        }
    }
    else if (InputMgr->m_kbState[SDL_SCANCODE_F2])
    {
        if (currentState == GameState::RUNNING)
        {
            m_stateMachine->TransitionTo(GameState::PAUSED);
            m_messageBox->ChangeMessage(m_stateMachine->GetCurrentStateAsString());

            if (m_textblockGenerator->IsRunning())
                m_textblockGenerator->ToggleRunning();
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
                m_textblockGenerator->ToggleRunning();
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
            m_textblockGenerator->ClearBlockDeque();
            m_textblockGenerator->SetTextBlockLimitReached(false);
        }
    }
}



