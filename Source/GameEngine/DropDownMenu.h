#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
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
        virtual ~DropDownMenu();
        void Draw() override;
        void Update(float dt) override;
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }
        void AddMenuItem(const std::string& name, std::unique_ptr<MenuItem> menuItem, Callback callback);
        bool AddCallback(const std::string& name, Callback callback);
        void InitializeMenu(int x, int y, int width, int height, Colors, bool = true);
        bool GetIsActive() const;
        void SetIsActive(bool);

    protected:
        void RespondToObserved(InputManager* InputMgr) override;
        void HandleMenuItem(InputManager* InputMgr, MenuItem* menuItem, const std::string& menuItemName, Callback callback);
        void InitializeCommonElements();
        virtual void InitializeMenuEntries() = 0;
        virtual void InitializeMenuEntry(MenuItem* menuItem, const std::string& label, int x, int y, float scale, int color) = 0;

        std::unique_ptr<RectangleDrawable> m_menuBody;
        std::unordered_map<std::string, MenuEntry> m_menuItems;

        bool m_isActive;
        int m_priority; // draw priority
    };
}
