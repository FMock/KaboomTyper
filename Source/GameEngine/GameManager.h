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

namespace GameEngine 
{
	class GameManager : public InputObserver
	{
	public:
		GameManager();
		~GameManager();
		void Update(float dt);
		void ProcessInput();
		void Render();
		bool ShouldQuit();

	private:
		std::unique_ptr<StateMachine> m_stateMachine;
		std::unique_ptr<InputTextBox> m_inputTextBox;
		std::shared_ptr<InputManager> m_inputManager; // shares with TextBlockManager
		std::unique_ptr<HeadsUpDisplay> m_headsUpDisplay;
		std::unique_ptr<Menu> m_gameMenu;
		std::unique_ptr<MessageBox> m_messageBox;

		TextBlockParameters m_textBlockParameters;
		std::unique_ptr<TextBlockManager> m_textblockGenerator;
		std::map<std::string, GLuint> m_stringToColoredBlockTextureMap;
		FontParameters m_fontParameters;
		std::shared_ptr<TextStringFont> m_fontPtr;
		std::map<GameState, LevelManager*> m_levels;

		void Initialize();
		void UpdateGameEntities(float deltaTime);

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}
