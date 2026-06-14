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
		if (m_menuItems.find(name) == m_menuItems.end())
		{
			m_itemOrder.push_back(name); // preserve insertion order for keyboard navigation
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
		m_menuBody->SetFrameColor(RGBColor::GetRGBColor(RGBColor::Red)); // red frame, blue fill

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
			int mouseX, mouseY;
			InputMgr->GetMousePosition(&mouseX, &mouseY);

			// Mouse hover updates the shared highlight; it does NOT clear it when
			// the cursor is over no item, so a keyboard highlight persists.
			for (int i = 0; i < static_cast<int>(m_itemOrder.size()); ++i)
			{
				MenuItem* item = m_menuItems[m_itemOrder[i]].menuItem.get();
				if (item && item->IsMouseOverMenuItem(mouseX, mouseY))
				{
					m_highlightedIndex = i;
					break;
				}
			}

			for (auto& item : m_menuItems)
			{
				HandleMenuItem(InputMgr, item.second.menuItem.get(), item.first, item.second.callback);
			}

			ApplyHighlight();
		}
	}

	void DropDownMenu::HandleMenuItem(InputManager* InputMgr, MenuItem* menuItem, const std::string& menuItemName, Callback callback)
	{
		int mouseX, mouseY;
		InputMgr->GetMousePosition(&mouseX, &mouseY);

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

	void DropDownMenu::ApplyHighlight()
	{
		for (int i = 0; i < static_cast<int>(m_itemOrder.size()); ++i)
		{
			MenuItem* item = m_menuItems[m_itemOrder[i]].menuItem.get();
			if (item)
				item->SetIsActive(i == m_highlightedIndex);
		}
	}

	int DropDownMenu::GetItemCount() const
	{
		return static_cast<int>(m_itemOrder.size());
	}

	void DropDownMenu::MoveHighlight(int delta)
	{
		int count = GetItemCount();
		if (count == 0)
			return;

		if (m_highlightedIndex < 0)
			m_highlightedIndex = (delta >= 0) ? 0 : count - 1;
		else
			m_highlightedIndex = ((m_highlightedIndex + delta) % count + count) % count;

		ApplyHighlight();
	}

	void DropDownMenu::SetHighlight(int index)
	{
		int count = GetItemCount();
		if (count == 0)
			m_highlightedIndex = -1;
		else if (index < 0)
			m_highlightedIndex = -1;
		else if (index >= count)
			m_highlightedIndex = count - 1;
		else
			m_highlightedIndex = index;

		ApplyHighlight();
	}

	void DropDownMenu::ResetHighlight()
	{
		m_highlightedIndex = -1;
		ApplyHighlight();
	}

	void DropDownMenu::ActivateHighlighted()
	{
		if (m_highlightedIndex < 0 || m_highlightedIndex >= GetItemCount())
			return;

		auto& entry = m_menuItems[m_itemOrder[m_highlightedIndex]];
		if (entry.menuItem && entry.callback)
			entry.callback(entry.menuItem->GetLabelText()); // mirror the mouse-click path
	}

	bool DropDownMenu::IsPointInBody(int x, int y) const
	{
		if (!m_menuBody)
			return false;

		int left = m_menuBody->GetXPosition();
		int top = m_menuBody->GetYPosition();
		return x >= left && x <= left + m_menuBody->GetWidth() &&
			   y >= top && y <= top + m_menuBody->GetHeight();
	}

	void DropDownMenu::InitializeCommonElements()
	{
		m_menuBody = std::make_unique<RectangleDrawable>();
	}
}
