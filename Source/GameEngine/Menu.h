#pragma once

#include "InputObserver.h"
#include "InputManager.h"
#include "RectangleDrawable.h"
#include "TextString.h"
#include "Button.h"
#include <memory>
#include <functional>

namespace GameEngine 
{
	class Menu : public InputObserver
	{
	public:

		enum MenuButtons
		{
			File,
			Options,
			Help,
			About,
			ButtonCount
		};

		Menu();
		~Menu();
		void Draw();
		void Update();

		using Callback = std::function<void(Menu::MenuButtons)>;
		void AddCallback(Callback callback, Menu::MenuButtons button);

	private:
		std::unique_ptr<RectangleDrawable> m_menuBar;
		std::unique_ptr<Button> m_fileBtn;
		std::unique_ptr<Button> m_optionsBtn;
		std::unique_ptr<Button> m_aboutBtn;
		std::unique_ptr<Button> m_helpBtn;
		TextString m_title;
		std::unique_ptr<RectangleDrawable> m_footer;

		void Initialize();

		Callback m_optionsBtnCallback;
		void OptionsButtonClicked();
		Callback m_fileBtnCallback;
		void FileButtonClicked();
		Callback m_helpBtnCallback;
		void HelpButtonClicked();
		Callback m_aboutBtnCallback;
		void AboutButtonClicked();
		void HandleButton(InputManager* InputMgr, Button* button, const std::string& buttonName, std::function<void()> callback);

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}