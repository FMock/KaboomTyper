#pragma once

#include "InputObserver.h"
#include "InputManager.h"
#include "RectangleDrawable.h"
#include "TextString.h"
#include "Button.h"
#include <memory>

namespace GameEngine 
{
	class Menu : public InputObserver
	{
	public:
		Menu();
		~Menu();
		void Draw();
		void Update();

	private:
		std::unique_ptr<RectangleDrawable> m_menuBar;
		std::unique_ptr<Button> m_fileBtn;
		std::unique_ptr<Button> m_optionsBtn;
		std::unique_ptr<Button> m_aboutBtn;
		std::unique_ptr<Button> m_helpBtn;

		void Initialize();

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}