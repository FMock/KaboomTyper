#include "GameManager.h"
#include "Utilities.h"
#include "LevelSplashScreen.h"
#include "LevelMainMenu.h"
#include "LevelGamePlay.h"
#include "LevelGameOver.h"
#include "TextBlock.h"

using namespace GameEngine;
using namespace GameEngine::Utility;

void GameManager::Initialize()
{
	// Load Splash Screen
	auto gameState = GameState::SPLASH_SCREEN;
	m_levels[gameState] = new LevelManager();
	auto levelSplashScreen = std::make_shared<LevelSplashScreen>();

	// Access m_spriteFactoryPtr to create a SplashScreen TextBlock
	auto& spriteFactoryPtr = levelSplashScreen->GetSpriteFactory();
	if (spriteFactoryPtr)
	{
		auto textBlock = spriteFactoryPtr->CreateSprite(SpriteType::TextBlock);

		if (auto* textBlockPtr = dynamic_cast<TextBlock*>(textBlock.get()))
		{
			// Access TextBlock-specific methods
			textBlockPtr->InitializeTextBlock(150, 150, std::string("Splash Screen"));
		}
	}
	m_levels[gameState]->AddLevel(levelSplashScreen);

	// Load Main Menu

	// Load Levels

	// Game Over
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
	// Updates level depending on current state
}

void GameManager::Render()
{
	// Renders level depending on current state
	m_levels[GameState::SPLASH_SCREEN]->GetLevel(0)->Render();
}

void GameManager::RespondToObserved(InputManager* InputMgr)
{

}
