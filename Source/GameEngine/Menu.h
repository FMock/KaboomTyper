/*
* menu.h 
* Abstract class for creating different kinds of menus
*/
#pragma once

#include "InputObserver.h"
#include "InputManager.h"
#include "RectangleDrawable.h"
#include "IDrawable.h"
#include "TextString.h"
#include "Button.h"
#include <memory>
#include <functional>
#include <unordered_map>

#define DEBUG_MENU 0

namespace GameEngine
{
    class Menu : public InputObserver, public IDrawable
    {
    public:
        using Callback = std::function<void(const std::string&)>;

        struct MenuEntry
        {
            std::unique_ptr<Button> button;
            Callback callback;
        };

        Menu();
        virtual ~Menu();
        void Draw() override;
        void Update(float dt) override;
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }
        void AddMenuItem(const std::string& name, std::unique_ptr<Button> button, Callback callback);
        bool AddCallback(const std::string& name, Callback callback);
        void InitializeMenu(const std::string& name, int nameX, int nameY, int x, int y, int width, int height, Colors, bool = true);

    protected:
        void RespondToObserved(InputManager* InputMgr) override;
        void HandleButton(InputManager* InputMgr, Button* button, const std::string& buttonName, Callback callback);
        void InitializeCommonElements();
        virtual void InitializeMenuItems() = 0;
        virtual void InitializeMenuItem(Button* button, const std::string& label, int x, int y, float scale, int color) = 0;

        std::unique_ptr<RectangleDrawable> m_menuBar;
        std::unique_ptr<RectangleDrawable> m_footer;
        std::unordered_map<std::string, MenuEntry> m_menuItems;
        TextString m_title;
        int m_priority; // draw priority
    };
}
