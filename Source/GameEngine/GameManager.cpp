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
	Utilities::ReadXmlFile("../../Config/FontParameters.xml", m_fontParameters); // TODO: DON'T USE HARD-CODED PATHS
	m_fontPtr = std::make_shared<TextStringFont>();
	m_fontPtr->image = m_fontParameters.m_texture;
	m_fontPtr->imageWidth = m_fontParameters.m_fontsheetWidth;
	m_fontPtr->imageHeight = m_fontParameters.m_fontsheetHeight;
	m_fontPtr->frameWidth = m_fontParameters.m_fontWidth;
	m_fontPtr->frameHeight = m_fontParameters.m_fontHeight;

	Utilities::ReadXmlFile("../../Config/TextBlockParameters.xml", m_textBlockParameters);

	// Load all the textures into the String-to-Image map
	m_stringToColoredBlockTextureMap["red"] = m_textBlockParameters.redBlockTexture;
	m_stringToColoredBlockTextureMap["blue"] = m_textBlockParameters.blueBlockTexture;
	m_stringToColoredBlockTextureMap["green"] = m_textBlockParameters.greenBlockTexture;
	m_stringToColoredBlockTextureMap["yellow"] = m_textBlockParameters.yellowBlockTexture;
	m_stringToColoredBlockTextureMap["purple"] = m_textBlockParameters.purpleBlockTexture;
	m_stringToColoredBlockTextureMap["white"] = m_textBlockParameters.whiteBlockTexture;
	m_stringToColoredBlockTextureMap["orange"] = m_textBlockParameters.orangeBlockTexture;

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
			textBlockPtr->InitializeTextBlock(150, 150, m_textBlockParameters, *m_fontPtr, std::string("Splash Screen"), m_stringToColoredBlockTextureMap);
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
