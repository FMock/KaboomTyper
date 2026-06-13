/*
* DropDownMenu.h
* Base class for creating different kinds of DropDownMenus
* DropDownMenus MenuItems are click-aware. Each MenuItems's OnClick behavior gets defined
* via the Callback passed to the HandleButton method.
*/
#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include "InputObserver.h"
#include "InputManager.h"
#include "IDrawable.h"
#include "RectangleDrawable.h"
#include "MenuItem.h"

#define DEBUG_DROPDOWNMENU 0

namespace GameEngine
{
    class DropDownMenu : public InputObserver, public IDrawable
    {
    public:

        using Callback = std::function<void(const std::string&)>;

        struct MenuEntry
        {
            std::unique_ptr<MenuItem> menuItem;
            Callback callback;
        };

        DropDownMenu();
        virtual ~DropDownMenu() = default;
        void Draw() override;
        void Update(float dt) override;
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }
        void AddMenuItem(const std::string& name, std::unique_ptr<MenuItem> menuItem, Callback callback);
        bool AddCallback(const std::string& name, Callback callback);
        void InitializeMenu(int x, int y, int width, int height, Colors, bool = true);
        bool GetIsActive() const;
        void SetIsActive(bool);

        // Keyboard navigation support
        int GetItemCount() const;
        void MoveHighlight(int delta);   // step the highlight by +/-1, wrapping
        void SetHighlight(int index);    // clamp into [0, count) (or -1 to clear)
        void ResetHighlight();           // clear the highlight (-1)
        void ActivateHighlighted();      // invoke the highlighted item's callback
        bool IsPointInBody(int x, int y) const; // hit-test against the menu body

    protected:
        void RespondToObserved(InputManager* InputMgr) override;
        void HandleMenuItem(InputManager* InputMgr, MenuItem* menuItem, const std::string& menuItemName, Callback callback);
        void InitializeCommonElements();
        void ApplyHighlight(); // mark only the highlighted item active
        virtual void InitializeMenuEntries() = 0;
        virtual void InitializeMenuEntry(MenuItem* menuItem, const std::string& label, int x, int y, float scale, int color) = 0;

        std::unique_ptr<RectangleDrawable> m_menuBody;
        std::unordered_map<std::string, MenuEntry> m_menuItems;
        std::vector<std::string> m_itemOrder; // insertion order of item keys (for keyboard nav)

        bool m_isActive;
        int m_highlightedIndex = -1; // shared mouse/keyboard highlight; -1 = none
        int m_priority; // draw priority
    };
}
