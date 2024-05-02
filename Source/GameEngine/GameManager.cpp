#include "GameManager.h"
#include "Utilities.h"
#include "LevelSplashScreen.h"
#include "LevelMainMenu.h"
#include "LevelGamePlay.h"
#include "LevelGameOver.h"
#include "TextBlock.h"

using namespace GameEngine;
using namespace GameEngine::Utility;

// Loads all the parts of the game
void GameManager::Initialize()
{
	// Initialize the InputTextbox
	m_inputTextBox = std::make_unique<InputTextBox>();
	m_inputTextBox->InitializeTextBox(10, 900, 780, 32);

	// Initialize Input Manager
	m_inputManager = std::make_unique<InputManager>();
	m_inputManager->RegisterObserver(m_inputTextBox.get()); // so InputTextbox can respond to user key presses
}

GameEngine::GameManager::GameManager() : m_gameState(GameState::SPLASH_SCREEN)
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
	m_inputManager->Update();
}

void GameManager::ProcessInput()
{

}

void GameManager::Render()
{
	m_inputTextBox->Draw();
}

bool GameEngine::GameManager::ShouldQuit()
{
	return m_inputManager->ShouldQuit();
}

void GameManager::RespondToObserved(InputManager* InputMgr)
{

}
