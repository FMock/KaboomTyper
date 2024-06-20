#pragma once
#include "InputObserver.h"
#include "GameStates.h"
#include "StateMachine.h"
#include "LevelManager.h"
#include "TextStringFont.h"
#include "FontParameters.h"
#include "TextBlockParameters.h"
#include <memory>
#include <vector>
#include <map>
#include "GameStates.h"
#include "InputTextBox.h"
#include "InputManager.h"
#include "Menu.h"
#include "MessageBox.h"
#include "HeadsUpDisplay.h"
#include "TextBlockManager.h"
#include "Firework.h"
#include "Color.h"
#include "DecorativeRectangleDrawable.h"
#include "GameAudio.h"
#include "GamePlayArea.h"
#include <memory>

/// <summary>
/// GameManager manages the various entities in the game to create a
/// pleasant and cohesive user experience.
/// </summary>

namespace GameEngine 
{
	class GameManager : public InputObserver, public std::enable_shared_from_this<GameManager>
	{
	public:
		static std::shared_ptr<GameManager> Create();
		~GameManager();
		void Update(float dt);
		void ProcessInput();
		void DisplayMenuChoices(Menu::MenuButtons);
		void Render();
		bool ShouldQuit();

	private:
		GameManager();
		void Initialize();
		void UpdateGameEntities(float deltaTime);
		void GameOver();

		std::shared_ptr<StateMachine> m_stateMachine;
		std::shared_ptr<InputTextBox> m_inputTextBox;
		std::shared_ptr<InputManager> m_inputManager; // shares with TextBlockManager
		std::unique_ptr<HeadsUpDisplay> m_headsUpDisplay;
		std::shared_ptr<Menu> m_gameMenu;
		std::unique_ptr<MessageBox> m_messageBox;
		GamePlayArea m_gamePlayArea;
		GLuint m_fireworkColorTexture;
		std::unique_ptr<Firework> m_firework;
		std::unique_ptr<Color> m_colorPtr;
		bool m_blowUpTextBlock;

		TextBlockParameters m_textBlockParameters;
		std::unique_ptr<TextBlockManager> m_textblockManager;
		std::map<std::string, GLuint> m_stringToColoredBlockTextureMap;
		FontParameters m_fontParameters;
		std::shared_ptr<TextStringFont> m_fontPtr;
		std::map<GameState, LevelManager*> m_levels;
		std::unique_ptr<DecorativeRectangle> m_rectangleOfRectangles;

		// Audio
		std::shared_ptr<GameAudio> m_explosion;
		std::string m_explosionPath;

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}
