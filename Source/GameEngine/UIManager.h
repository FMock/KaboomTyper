#pragma once

#include <memory>
#include <functional>
#include <utility> //for std::forward 
#include <map>
#include <string>
#include "Menu.h"
#include "InputTextBox.h"
#include "GamePlayArea.h"
#include "HeadsUpDisplay.h"
#include "MessageBox.h"
#include "DropDownMenu.h"
#include "FileDropDownMenu.h"
#include "OptionsDropDownMenu.h"
#include "ChoiceMenu.h"
#include "WordCategoryChoiceMenu.h"
#include "WordManager.h"
#include "InputManager.h"
#include "DrawOrderManager.h"
#include "InputMessageBox.h"

namespace GameEngine
{
	class UIManager
	{
	public:
		UIManager(std::shared_ptr<InputManager> inputManager, std::shared_ptr<WordManager> wordManager);
		void Update(float dt);
		void ProcessInput();
		void Render();
		void ResetScore();
		void IncreaseScore();
		void GameOver();
		void DisplayMainMenuChoices(const std::string& buttonName);
		void WordCategoryChoiceMenuOnClick(const std::string& choice);
		void FileDropDownMenuOnClick(const std::string& choice);
		void OptionsDropDownMenuOnClick(const std::string& choice);
		void RegisterDrawables(DrawOrderManager&  manager);
		void GetUserNamePromptCallback();
		void CancelButtonCallback();
		void ChangeStartMenuItemLabel(const std::string& choice);
		using Callback = std::function<void()>;
		void AddCallback(Callback callback);
		void AddGameOverCallback(Callback callback);
		void AddStartGameCallback(Callback callback);

		template<typename... Args> // Wrapper for MessageBox::ChangeMessage(Args&&... args)
		void ChangeMessageBoxMessage(Args&&... args);

	private:
		std::map<std::string, std::shared_ptr<DropDownMenu>> m_dropDownMenus;
		std::map<std::string, std::shared_ptr<ChoiceMenu>> m_choiceMenus;
		std::shared_ptr<InputTextBox> m_inputTextBox;
		std::shared_ptr<GamePlayArea> m_gamePlayArea;
		std::shared_ptr<HeadsUpDisplay> m_headsUpDisplay;
		std::shared_ptr<MessageBox> m_messageBox;
		std::shared_ptr<Menu> m_gameMenu;
		std::shared_ptr<InputManager> m_inputManager;
		std::shared_ptr<InputMessageBox> m_inputMessageBox;
		std::shared_ptr<WordManager> m_wordManager;
		std::vector<std::string> m_wordCategories;
		Callback m_processInputCallback;
		Callback m_gameOverCallback;
		Callback m_startGameCallback;
		int m_priority; // draw priority
		bool m_initialized;
		void Initialize();
		bool RegisterCallbacks();
		void DisableAllButtonsExceptThisButton(const std::string& buttonName);
	};

	template<typename... Args>
	void UIManager::ChangeMessageBoxMessage(Args&&... args)
	{
		m_messageBox->ChangeMessage(std::forward<Args>(args)...);
	}
}
