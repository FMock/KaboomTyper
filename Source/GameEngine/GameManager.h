#pragma once
#include "InputObserver.h"
#include "GameStates.h"
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
		std::unique_ptr<InputTextBox> m_inputTextBox;
		std::unique_ptr<InputManager> m_inputManager;
		GameState m_gameState;
		TextBlockParameters m_textBlockParameters;
		std::map<std::string, GLuint> m_stringToColoredBlockTextureMap;
		FontParameters m_fontParameters;
		std::shared_ptr<TextStringFont> m_fontPtr;
		std::map<GameState, LevelManager*> m_levels;
		std::unique_ptr<Menu> m_gameMenu;
		void Initialize();

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}


