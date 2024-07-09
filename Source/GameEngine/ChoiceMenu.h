#pragma once

#include "ChoiceMenuItem.h"
#include <unordered_map>
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
        virtual ~ChoiceMenu();
        void Draw() override;
        void Update(float dt) override;
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }
        void AddChoiceMenuItem(const std::string& name, std::unique_ptr<ChoiceMenuItem> menuItem, Callback callback);
        bool AddCallback(const std::string& name, Callback callback);
        void InitializeChoiceMenu(int x, int y, int width, int height, Colors, bool = true);
        bool GetIsActive() const;
        void SetIsActive(bool);

    protected:
        void RespondToObserved(InputManager* InputMgr) override;
        void HandleMenuItem(InputManager* InputMgr, ChoiceMenuItem* menuItem, const std::string& menuItemName, Callback callback);
        void InitializeCommonElements();
        virtual void InitializeMenuEntries() = 0;
        virtual void InitializeMenuEntry(ChoiceMenuItem* menuItem, const std::string& label, int x, int y, float scale, int color) = 0;

        std::unique_ptr<RectangleDrawable> m_menuBody;
        std::unordered_map<std::string, ChoiceMenuEntry> m_choiceMenuItems;
        int m_count; // number of ChoiceMenuItems
        bool m_isActive;
        int m_priority; // draw priority
	};
}
