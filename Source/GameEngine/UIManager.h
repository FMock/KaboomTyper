#pragma once

#include <memory>
#include <utility> //for std::forward 
#include "GamePlayArea.h"
#include "HeadsUpDisplay.h"
#include "MessageBox.h"
#include "Menu.h"
#include "FileContextMenu.h"
#include "InputManager.h"
#include "DrawOrderManager.h"

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
		void DisplayMenuChoices(Menu::MenuButtons button);
		void DisplayFileMenuChoices(FileContextMenu::Choices button);
		void RegisterDrawables(DrawOrderManager&  manager);

		template<typename... Args> // Wrapper for MessageBox::ChangeMessage(Args&&... args)
		void ChangeMessageBoxMessage(Args&&... args);


	private:
		std::shared_ptr<GamePlayArea> m_gamePlayArea;
		std::shared_ptr<HeadsUpDisplay> m_headsUpDisplay;
		std::shared_ptr<MessageBox> m_messageBox;
		std::shared_ptr<FileContextMenu> m_fileContextMenu;
		std::shared_ptr<Menu> m_gameMenu;
		std::shared_ptr<InputManager> m_inputManager;

		bool m_initialized;
		void Initialize();
	};

	template<typename... Args>
	void UIManager::ChangeMessageBoxMessage(Args&&... args)
	{
		m_messageBox->ChangeMessage(std::forward<Args>(args)...);
	}
}
