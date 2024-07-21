#include "ChoiceMenu.h"
#include "Common.h"
#include <algorithm>
#include <iostream>

namespace GameEngine
{

    ChoiceMenu::ChoiceMenu() : m_isActive(false), m_priority(0), m_count(0)
    {
        SetIsActive(false);
        InitializeCommonElements();
    }

    void ChoiceMenu::Draw()
    {
        if (!GetIsActive()) return;

        m_menuBody->Draw();

        for (const auto& item : m_choiceMenuItems)
        {
            if (item.second.menuItem)
            {
#if DEBUG_CHOICEMENU
                // std::cout << "Drawing ChoiceMenuItem: " << item.first << std::endl;
#endif
                item.second.menuItem->Draw();
            }
            else
            {
                std::cerr << "Error: Null choice menu item pointer for item: " << item.first << std::endl;
            }
        }
    }

    void ChoiceMenu::Update(float dt)
    {
        for (auto& item : m_choiceMenuItems)
        {
            if (item.second.menuItem)
            {
                item.second.menuItem->Update(dt);
            }
        }
    }

    void ChoiceMenu::AddChoiceMenuItem(const std::string& name, std::unique_ptr<ChoiceMenuItem> choiceMenuItem, Callback callback)
    {
        if (!choiceMenuItem)
        {
            std::cerr << "Error: Null ChoiceMenuItem passed for choiceMenuItem: " << name << std::endl;
            return; // Early exit if choiceMenuItem is null
        }

        m_choiceMenuItems[name] = ChoiceMenuEntry{ std::move(choiceMenuItem), std::move(callback) };
        m_choiceMenuItems[name].menuItem->SetName(name);

        if (name == "Default")
            m_choiceMenuItems["Default"].menuItem->SetIsSelected(true);
    }

    bool ChoiceMenu::AddCallback(const std::string& name, Callback callback)
    {
        auto it = m_choiceMenuItems.find(name);
        if (it != m_choiceMenuItems.end())
        {
            it->second.callback = callback;
            return true;
        }

#if DEBUG_CHOICEMENU
        std::cout << "Warning: Attempting to add callback to non-existent choice menu item: " << name << std::endl;
#endif

        return false;
    }

    void ChoiceMenu::InitializeChoiceMenu(int x, int y, int width, int height, Colors color, bool flag)
    {
        m_menuBody->Initialize(x, y, width, height, color, flag);

        const int verticalSpacer = 32;

        // Determine the largest string size
        size_t max_size = std::max_element(
            m_choiceMenuItems.begin(), m_choiceMenuItems.end(),
            [](const auto& a, const auto& b) { return a.first.size() < b.first.size(); }
        )->first.size();

        // Adjust the size of this choice menu based on its contents
        int menuWidth = static_cast<int>(max_size * 21);
        int menuHeight = static_cast<int>(m_choiceMenuItems.size() * (Common::FONT_HEIGHT * 1.30));
        SetWidth(menuWidth);

        if(m_choiceMenuItems.size() == 1) // Ensure menu is large enough when menu only contains a single choice
            SetHeight(100);
        else
            SetHeight(menuHeight);

        // Initialize choice menu items
        for (auto& item : m_choiceMenuItems)
        {
            int augment_size = static_cast<int>(max_size - item.first.size());
            auto& menuItem = item.second.menuItem;

            menuItem->SetAugmentedSize(augment_size);
            InitializeMenuEntry(menuItem.get(), item.first, x, y + verticalSpacer, 1.0f, Colors::DEFAULT_COLOR);
            menuItem->SetXPosition(x);
            menuItem->SetYPosition(y + verticalSpacer);
            menuItem->AugmentLabel(augment_size);

            y += static_cast<int>(Common::FONT_HEIGHT * 1.25);
        }
    }

    bool ChoiceMenu::GetIsActive() const
    {
        return m_isActive;
    }

    void ChoiceMenu::SetIsActive(bool isActive)
    {
        m_isActive = isActive;
    }

    int ChoiceMenu::GetWidth() const
    {
        return m_menuBody->GetWidth();
    }

    int ChoiceMenu::GetHeight() const
    {
        return m_menuBody->GetHeight();
    }

    void ChoiceMenu::SetWidth(int width)
    {
        if (width > 0)
            m_menuBody->SetWidth(width);
    }

    void ChoiceMenu::SetHeight(int height)
    {
        if (height > 0)
            m_menuBody->SetHeight(height);
    }

    bool ChoiceMenu::GetMenuItemSelectionState(const std::string& key) const
    {
        bool state = false;
        auto choiceMenuItem = m_choiceMenuItems.find(key);
        if (choiceMenuItem != m_choiceMenuItems.end())
        {
            state = choiceMenuItem->second.menuItem->GetIsSelected();
        }
        return state;
    }

    void ChoiceMenu::RespondToObserved(InputManager* InputMgr)
    {
        if (GetIsActive())
        {
            for (auto& item : m_choiceMenuItems)
            {
                HandleMenuItem(InputMgr, item.second.menuItem.get(), item.first, item.second.callback);
            }
        }
    }

    // Handles logic related to selecting and de-selcting a ChoiceMenuItem
    // Requirements:
    // When "Default" menu item is selected, no other item can be selected
    // When the last item selected is de-selected, "Default" gets selected automatically
    void ChoiceMenu::HandleMenuItem(InputManager* InputMgr, ChoiceMenuItem* menuItem, const std::string& menuItemName, Callback callback)
    {
        int mouseX, mouseY;
        InputMgr->GetMousePosition(&mouseX, &mouseY);

        bool isHovered = menuItem->MouseHoverPresent(mouseX, mouseY);
        menuItem->SetIsActive(isHovered);

        bool mouseState = InputMgr->m_mouseButtonState[0];
        bool prevMouseState = InputMgr->m_prevMouseButtonState[0];

        if (mouseState && !prevMouseState && isHovered)
        {
#if DEBUG_CHOICEMENU
            std::cout << menuItem << " ChoiceMenuItem clicked" << std::endl;
#endif
        }
        else if (!mouseState && prevMouseState && isHovered)
        {
#if DEBUG_CHOICEMENU
            std::cout << menuItem << " ChoiceMenuItem released" << std::endl;
#endif
            bool wasSelected = menuItem->GetIsSelected();
            menuItem->SetIsSelected(!wasSelected);

            if (menuItem->GetIsSelected())
            {
                // Deselect all other menu items
                for (auto& item : m_choiceMenuItems)
                {
                    if (item.first != menuItemName)
                    {
                        item.second.menuItem->SetIsSelected(false);
                    }
                }
            }
            else
            {
                // Ensure Default is selected if all others are deselected
                bool anySelected = false;
                for (const auto& item : m_choiceMenuItems)
                {
                    if (item.second.menuItem->GetIsSelected())
                    {
                        anySelected = true;
                        break;
                    }
                }

                if (!anySelected)
                {
                    auto defaultItem = m_choiceMenuItems.find("Default");
                    if (defaultItem != m_choiceMenuItems.end())
                    {
                        defaultItem->second.menuItem->SetIsSelected(true);
                    }
                }
            }

            callback(menuItemName);

#if DEBUG_CHOICEMENU
            std::cout << menuItem->GetName() << " is Selected = " << menuItem->GetIsSelected() << ", is Active = " << menuItem->GetIsActive() << std::endl;
#endif
        }
    }

    void ChoiceMenu::InitializeCommonElements()
    {
        m_menuBody = std::make_unique<RectangleDrawable>();
    }
}
