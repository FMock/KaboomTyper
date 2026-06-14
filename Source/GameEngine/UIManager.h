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
#include "HelpDropDownMenu.h"
#include "AboutDropDownMenu.h"
#include "ChoiceMenu.h"
#include "WordCategoryChoiceMenu.h"
#include "WordManager.h"
#include "InputManager.h"
#include "DrawOrderManager.h"
#include "InputMessageBox.h"
#include "PopupMessageBox.h"

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

		// Keyboard + click-outside handling for the top menu bar.
		// Returns true if it consumed input this frame (so the caller can skip its own handling).
		bool HandleMenuInput(InputManager* InputMgr);
		bool IsAnyMenuOpen() const;
		void CloseAllMenus();

		// Modal popups (exit confirmation + Help windows).
		void ShowExitConfirm();          // pop the "really quit?" dialog
		bool IsAnyModalActive() const;   // true if any popup dialog is showing
		void CloseActiveModal();         // dismiss whichever popup is showing
		void OpenWordCategoryFlyout(const std::string& name); // open a fly-out beside the Word Category menu

		void WordCategoryChoiceMenuOnClick(const std::string& choice);
		void AudioChoiceMenuOnClick(const std::string& choice);
		void WordSpeedChoiceMenuOnClick(const std::string& choice);
		void FileDropDownMenuOnClick(const std::string& choice);
		void OptionsDropDownMenuOnClick(const std::string& choice);
		void HelpDropDownMenuOnClick(const std::string& choice);
		void RegisterDrawables(DrawOrderManager&  manager);
		void GetUserNamePromptCallback();
		void CancelButtonCallback();
		void ChangeStartMenuItemLabel(const std::string& choice);
		using Callback = std::function<void()>;
		using AudioCallback = std::function<void(bool)>;
		using WordSpeedCallback = std::function<void(const std::string&)>;
		void AddCallback(Callback callback);
		void AddGameOverCallback(Callback callback);
		void AddStartGameCallback(Callback callback);
		void AddAudioCallback(AudioCallback callback);
		void AddWordSpeedCallback(WordSpeedCallback callback);

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
		std::shared_ptr<PopupMessageBox> m_exitConfirmPopup;
		std::shared_ptr<PopupMessageBox> m_instructionsPopup;
		std::shared_ptr<PopupMessageBox> m_feedbackPopup;
		std::shared_ptr<PopupMessageBox> m_sourcePopup;
		std::shared_ptr<WordManager> m_wordManager;
		std::vector<std::string> m_wordCategories;     // all categories from the DB
		std::vector<std::string> m_animalCategories;   // categories grouped under "Animals"
		std::vector<std::string> m_topLevelCategories; // "Animals" + the non-animal categories
		std::vector<std::string> m_audioOptions;
		std::vector<std::string> m_wordSpeedOptions;
		Callback m_processInputCallback;
		Callback m_gameOverCallback;
		Callback m_startGameCallback;
		WordSpeedCallback m_wordSpeedCallback;
		AudioCallback m_audioCallback;
		int m_priority; // draw priority
		bool m_initialized;
		void Initialize();
		bool RegisterCallbacks();
		void DisableAllButtonsExceptThisButton(const std::string& buttonName);
		// Show a popup, ensuring it is the only one visible (closes any other first).
		void ShowModal(const std::shared_ptr<PopupMessageBox>& popup);

		// Keyboard-navigation helpers
		void OpenMenu(const std::string& name);              // open this dropdown, close others, highlight first item
		std::shared_ptr<DropDownMenu> GetOpenDropDown() const; // the active dropdown, or nullptr
		std::shared_ptr<ChoiceMenu> GetOpenChoiceMenu() const; // the active Options sub-menu, or nullptr
		std::shared_ptr<ChoiceMenu> GetFocusedChoiceMenu() const; // keyboard focus: fly-out wins over its parent
		int ComputeFlyoutX(int parentX, int parentWidth, int flyoutWidth) const; // off-screen-safe left/right placement
		// Visual left-to-right order of the drop-down top menus (About is modal, excluded)
		std::vector<std::string> m_topMenuOrder{ "File", "Options", "Help", "About" };
	};

	template<typename... Args>
	void UIManager::ChangeMessageBoxMessage(Args&&... args)
	{
		m_messageBox->ChangeMessage(std::forward<Args>(args)...);
	}
}
