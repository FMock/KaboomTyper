#include "DropDownMenu.h"

namespace GameEngine
{
	DropDownMenu::DropDownMenu() : m_priority(0)
	{
		SetIsActive(false);
		InitializeCommonElements();
	}

	void DropDownMenu::Draw()
	{
		if (GetIsActive()) 
		{
			m_menuBody->Draw();

			// Draw all menu items
			for (const auto& item : m_menuItems)
			{
				if (item.second.menuItem)
				{
#if DEBUG_DROPDOWNMENU
					//std::cout << "Drawing MenuItem: " << item.first << std::endl;
#endif
					item.second.menuItem->Draw();
			}
				else
				{
					std::cerr << "Error: Null menu item pointer for item: " << item.first << std::endl;
				}
		}
		}
	}

	void DropDownMenu::Update(float dt)
	{
		// Update all MenuItems
		for (auto& item : m_menuItems)
		{
			if (item.second.menuItem)
			{
				item.second.menuItem->Update(dt);
			}
		}

		// if game state == stop, change SetLabelText("START") and set flag to stop the game

		// if game state == run, change SetLabelText("STOP") and set flag to start the game
	}

	void DropDownMenu::AddMenuItem(const std::string& name, std::unique_ptr<MenuItem> menuItem, Callback callback)
	{
#if DEBUG_DROPDOWNMENU
		std::cout << "Adding menu item: " << name << std::endl;
#endif

		if (!menuItem)
		{
			std::cerr << "Error: Null menuItem passed for menu item: " << name << std::endl;
		}
		m_menuItems[name] = MenuEntry{ std::move(menuItem), callback };
	}

	bool DropDownMenu::AddCallback(const std::string& name, Callback callback)
	{
		auto it = m_menuItems.find(name);
		if (it != m_menuItems.end())
		{
			it->second.callback = callback;
			return true;
		}
#if DEBUG_DROPDOWNMENU
		std::cout << "Warning: Attempting to add callback to non-existent menu item: " << name << std::endl;
#endif
		return false;
	}

	void DropDownMenu::InitializeMenu(int x, int y, int width, int height, Colors, bool)
	{
		m_menuBody->Initialize(x, y, width, height, Colors::BLUE, true);

		// Initialize menu items
		for (auto& item : m_menuItems)
		{
			InitializeMenuEntry(item.second.menuItem.get(), item.first, x, y, 1.0f, Colors::DEFAULT_COLOR); // Pass default values
		}
	}

	bool DropDownMenu::GetIsActive() const
	{
		return m_isActive;
	}

	void DropDownMenu::SetIsActive(bool isActive)
	{
		m_isActive = isActive;
	}

	void DropDownMenu::RespondToObserved(InputManager* InputMgr)
	{
		if (GetIsActive()) 
		{
			for (auto& item : m_menuItems)
			{
				HandleMenuItem(InputMgr, item.second.menuItem.get(), item.first, item.second.callback);
			}
		}
	}

	void DropDownMenu::HandleMenuItem(InputManager* InputMgr, MenuItem* menuItem, const std::string& menuItemName, Callback callback)
	{
		int mouseX, mouseY;
		InputMgr->GetMousePosition(&mouseX, &mouseY);

		menuItem->SetIsActive(menuItem->IsMouseOverMenuItem(mouseX, mouseY)); // toggle current menuItem active state

		if (InputMgr->m_mouseButtonState[0] && !InputMgr->m_prevMouseButtonState[0] && menuItem->IsMouseOverMenuItem(mouseX, mouseY))
		{
#if DEBUG_DROPDOWNMENU
			std::cout << menuItem << " MenuItem clicked" << std::endl;
#endif
			menuItem->SetMenuItemColor(Colors::DARK_GRAY);
			//callback(menuItemName);
			callback(menuItem->GetLabelText());
		}
		else if (!InputMgr->m_mouseButtonState[0] && InputMgr->m_prevMouseButtonState[0] && menuItem->IsMouseOverMenuItem(mouseX, mouseY))
		{
#if DEBUG_DROPDOWNMENU
			std::cout << menuItem << " MenuItem released" << std::endl;
#endif
			menuItem->SetMenuItemColor(Colors::DEFAULT_COLOR);
		}
	}

	void DropDownMenu::InitializeCommonElements()
	{
		m_menuBody = std::make_unique<RectangleDrawable>();
	}
}
