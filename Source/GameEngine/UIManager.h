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
#include "InputManager.h"
#include "DrawOrderManager.h"
#include "InputMessageBox.h"

namespace GameEngine
{
	class UIManager
	{
	public:
		UIManager(std::shared_ptr<InputManager> inputManager);
		~UIManager();
		void Update(float dt);
		void ProcessInput();
		void Render();
		void ResetScore();
		void IncreaseScore();
		void GameOver();
		void DisplayMenuChoices(const std::string& buttonName);
		void DisplayFileMenuChoices(FileDropDownMenu::Choices button);
		void DisplayOptionsMenuChoices(OptionsDropDownMenu::Choices button);
		void RegisterDrawables(DrawOrderManager&  manager);
		void GetUserNamePromptCallback();
		void CancelButtonCallback();
		using Callback = std::function<void()>;
		void AddCallback(Callback callback);

		template<typename... Args> // Wrapper for MessageBox::ChangeMessage(Args&&... args)
		void ChangeMessageBoxMessage(Args&&... args);

	private:
		std::map<std::string, std::shared_ptr<DropDownMenu>> m_dropDownMenus;
		std::shared_ptr<InputTextBox> m_inputTextBox;
		std::shared_ptr<GamePlayArea> m_gamePlayArea;
		std::shared_ptr<HeadsUpDisplay> m_headsUpDisplay;
		std::shared_ptr<MessageBox> m_messageBox;
		std::shared_ptr<Menu> m_gameMenu;
		std::shared_ptr<InputManager> m_inputManager;
		std::shared_ptr<InputMessageBox> m_inputMessageBox;
		Callback m_processInputCallback;
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
