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

        if (m_choiceMenuItems.find(name) == m_choiceMenuItems.end())
            m_itemOrder.push_back(name); // provisional order; sorted by Y after layout

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
        // Remember the layout args so the menu can be repositioned later (fly-out support).
        m_layoutX = x;
        m_layoutY = y;
        m_layoutWidth = width;
        m_layoutHeight = height;
        m_layoutColor = color;
        m_layoutFlag = flag;

        m_menuBody->Initialize(x, y, width, height, color, flag);
        m_menuBody->SetFrameColor(RGBColor::GetRGBColor(RGBColor::LightGray)); // light gray frame, keep fill

        const int verticalSpacer = 32;

        // Nothing to lay out if there are no items; keep the passed-in size and bail
        // (avoids dereferencing std::max_element's end() iterator on an empty container).
        if (m_choiceMenuItems.empty())
        {
            SetWidth(width);
            SetHeight(height);
            return;
        }

        // Determine the largest string size
        size_t max_size = std::max_element(
            m_choiceMenuItems.begin(), m_choiceMenuItems.end(),
            [](const auto& a, const auto& b) { return a.first.size() < b.first.size(); }
        )->first.size();

        // Adjust the size of this choice menu based on its contents
        int menuWidth = static_cast<int>(max_size * 21);
        int menuHeight = static_cast<int>(m_choiceMenuItems.size() * (Common::FONT_HEIGHT * 1.30));
        SetWidth(menuWidth);

        if (m_choiceMenuItems.size() == 1) // TODO: GENERALIZE CHOICE MENU SIZE FOR ALL SIZES
        {
            SetHeight(100);
        }
        else if (m_choiceMenuItems.size() == 3)
        {
            SetHeight(165);
            SetWidth(180);
        }
        else if (m_choiceMenuItems.size() == 4)
        {
            SetHeight(200);
            SetWidth(180);
        }
        else
        {
            SetHeight(menuHeight);
        }

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

        // Order the keyboard-navigation list top-to-bottom to match the on-screen layout
        // (items are laid out in unordered_map order above, so sort by final Y position).
        std::sort(m_itemOrder.begin(), m_itemOrder.end(),
            [this](const std::string& a, const std::string& b)
            {
                return m_choiceMenuItems[a].menuItem->GetYPostion() <
                       m_choiceMenuItems[b].menuItem->GetYPostion();
            });
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

    bool ChoiceMenu::IsPointInBody(int x, int y) const
    {
        if (!m_menuBody)
            return false;

        int left = m_menuBody->GetXPosition();
        int top = m_menuBody->GetYPosition();
        return x >= left && x <= left + m_menuBody->GetWidth() &&
               y >= top && y <= top + m_menuBody->GetHeight();
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
            int mouseX, mouseY;
            InputMgr->GetMousePosition(&mouseX, &mouseY);

            // Mouse hover updates the shared highlight; it does NOT clear it when the
            // cursor is over no item, so a keyboard highlight persists.
            for (int i = 0; i < static_cast<int>(m_itemOrder.size()); ++i)
            {
                ChoiceMenuItem* item = m_choiceMenuItems[m_itemOrder[i]].menuItem.get();
                if (item && item->MouseHoverPresent(mouseX, mouseY))
                {
                    m_highlightedIndex = i;
                    break;
                }
            }

            for (auto& item : m_choiceMenuItems)
            {
                HandleMenuItem(InputMgr, item.second.menuItem.get(), item.first, item.second.callback);
            }

            ApplyHighlight();
        }
    }


    void ChoiceMenu::HandleMenuItem(InputManager* InputMgr, ChoiceMenuItem* menuItem, const std::string& menuItemName, Callback callback)
    {
        int mouseX, mouseY;
        InputMgr->GetMousePosition(&mouseX, &mouseY);

        bool isHovered = menuItem->MouseHoverPresent(mouseX, mouseY);

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
            SelectItem(menuItemName); // shared with keyboard activation
        }
    }

    // Toggle/select the named item, applying the radio + "Default" fallback rules,
    // then fire its callback. Shared by mouse release and keyboard Enter.
    void ChoiceMenu::SelectItem(const std::string& menuItemName)
    {
        auto it = m_choiceMenuItems.find(menuItemName);
        if (it == m_choiceMenuItems.end())
            return;

        ChoiceMenuItem* menuItem = it->second.menuItem.get();
        Callback callback = it->second.callback;

        bool wasSelected = menuItem->GetIsSelected();
        std::string callbackMenuItemName = menuItemName;

        if (wasSelected)
        {
            // If the currently selected item is selected again
            auto defaultItem = m_choiceMenuItems.find("Default");
            if (defaultItem != m_choiceMenuItems.end())
            {
                // If "Default" item exists, select it
                menuItem->SetIsSelected(false);
                defaultItem->second.menuItem->SetIsSelected(true);
                callbackMenuItemName = "Default"; // Update the callback name
            }
            else
            {
                // If "Default" item does not exist, simply deselect the current item
                menuItem->SetIsSelected(false);
            }
        }
        else
        {
            menuItem->SetIsSelected(true);

            // Deselect all other menu items
            for (auto& item : m_choiceMenuItems)
            {
                if (item.first != menuItemName)
                {
                    item.second.menuItem->SetIsSelected(false);
                }
            }
        }

        // Ensure "Default" is selected if all others are deselected and it exists
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
                callbackMenuItemName = "Default"; // Update the callback name if Default is selected
            }
        }

        if (callback)
            callback(callbackMenuItemName);

#if DEBUG_CHOICEMENU
        std::cout << menuItem->GetName() << " is Selected = " << menuItem->GetIsSelected() << ", is Active = " << menuItem->GetIsActive() << std::endl;
#endif
    }

    void ChoiceMenu::ApplyHighlight()
    {
        for (int i = 0; i < static_cast<int>(m_itemOrder.size()); ++i)
        {
            ChoiceMenuItem* item = m_choiceMenuItems[m_itemOrder[i]].menuItem.get();
            if (item)
                item->SetIsActive(i == m_highlightedIndex);
        }
    }

    int ChoiceMenu::GetItemCount() const
    {
        return static_cast<int>(m_itemOrder.size());
    }

    void ChoiceMenu::MoveHighlight(int delta)
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

    void ChoiceMenu::SetHighlight(int index)
    {
        int count = GetItemCount();
        if (count == 0 || index < 0)
            m_highlightedIndex = -1;
        else if (index >= count)
            m_highlightedIndex = count - 1;
        else
            m_highlightedIndex = index;

        ApplyHighlight();
    }

    void ChoiceMenu::ResetHighlight()
    {
        m_highlightedIndex = -1;
        ApplyHighlight();
    }

    void ChoiceMenu::ActivateHighlighted()
    {
        if (m_highlightedIndex < 0 || m_highlightedIndex >= GetItemCount())
            return;

        SelectItem(m_itemOrder[m_highlightedIndex]);
    }

    std::string ChoiceMenu::GetHighlightedName() const
    {
        if (m_highlightedIndex < 0 || m_highlightedIndex >= static_cast<int>(m_itemOrder.size()))
            return "";

        return m_itemOrder[m_highlightedIndex];
    }

    int ChoiceMenu::GetMenuX() const
    {
        return m_menuBody ? m_menuBody->GetXPosition() : 0;
    }

    void ChoiceMenu::RepositionX(int newX)
    {
        // Re-run the layout at the new X (Y/size preserved). InitializeChoiceMenu is
        // idempotent: each item's Initialize resets its (augmented) label before the
        // layout re-augments it.
        InitializeChoiceMenu(newX, m_layoutY, m_layoutWidth, m_layoutHeight, m_layoutColor, m_layoutFlag);
    }


    void ChoiceMenu::InitializeCommonElements()
    {
        m_menuBody = std::make_unique<RectangleDrawable>();
    }
}
