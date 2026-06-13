#pragma once

#include "ChoiceMenuItem.h"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include "InputObserver.h"
#include "InputManager.h"
#include "IDrawable.h"
#include "RectangleDrawable.h"

#define DEBUG_CHOICEMENU 1

namespace GameEngine
{
	class ChoiceMenu : public InputObserver, public IDrawable
	{
    public:

        using Callback = std::function<void(const std::string&)>;

        struct ChoiceMenuEntry
        {
            std::unique_ptr<ChoiceMenuItem> menuItem;
            Callback callback;
        };

        ChoiceMenu();
        virtual ~ChoiceMenu() = default;
        void Draw() override;
        void Update(float dt) override;
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }
        void AddChoiceMenuItem(const std::string& name, std::unique_ptr<ChoiceMenuItem> menuItem, Callback callback);
        bool AddCallback(const std::string& name, Callback callback);
        void InitializeChoiceMenu(int x, int y, int width, int height, Colors, bool = true);
        bool GetIsActive() const;
        void SetIsActive(bool);
        int GetWidth() const;
        int GetHeight() const;
        void SetWidth(int width);
        void SetHeight(int height);
        bool GetMenuItemSelectionState(const std::string& key) const;
        bool IsPointInBody(int x, int y) const; // hit-test against the menu body

        // Keyboard navigation support (mirrors DropDownMenu)
        int GetItemCount() const;
        void MoveHighlight(int delta);   // step the highlight by +/-1, wrapping
        void SetHighlight(int index);    // clamp into [0, count) (or -1 to clear)
        void ResetHighlight();           // clear the highlight (-1)
        void ActivateHighlighted();      // select the highlighted item
        std::string GetHighlightedName() const; // key of the highlighted item, or ""

        // Fly-out positioning support
        int GetMenuX() const;            // current left edge of the menu body
        void RepositionX(int newX);      // move the whole menu horizontally (Y unchanged)

    protected:
        void RespondToObserved(InputManager* InputMgr) override;
        void HandleMenuItem(InputManager* InputMgr, ChoiceMenuItem* menuItem, const std::string& menuItemName, Callback callback);
        void SelectItem(const std::string& menuItemName); // shared mouse/keyboard selection logic
        void ApplyHighlight();                            // mark only the highlighted item active
        void InitializeCommonElements();
        virtual void InitializeMenuEntries() = 0;
        virtual void InitializeMenuEntry(ChoiceMenuItem* menuItem, const std::string& label, int x, int y, float scale, int color) = 0;

        std::unique_ptr<RectangleDrawable> m_menuBody;
        std::unordered_map<std::string, ChoiceMenuEntry> m_choiceMenuItems;
        std::vector<std::string> m_itemOrder; // top-to-bottom visual order (for keyboard nav)
        int m_count; // number of ChoiceMenuItems
        bool m_isActive;
        int m_highlightedIndex = -1; // shared mouse/keyboard highlight; -1 = none
        int m_priority; // draw priority
        int m_width;
        int m_height;

        // Saved layout args from the last InitializeChoiceMenu call, so the menu can be
        // repositioned later (used by fly-out menus).
        int m_layoutX = 0;
        int m_layoutY = 0;
        int m_layoutWidth = 0;
        int m_layoutHeight = 0;
        Colors m_layoutColor = Colors::BLUE;
        bool m_layoutFlag = true;
	};
}
