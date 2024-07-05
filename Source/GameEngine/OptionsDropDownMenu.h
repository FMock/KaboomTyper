#pragma once

#include "DropDownMenu.h"
#include "RectangleDrawable.h"
#include "IDrawable.h"
#include "MenuItem.h"
#include "InputManager.h"
#include <memory>
#include <functional>

namespace GameEngine
{
	class OptionsDropDownMenu : public DropDownMenu, public IDrawable
	{
    public:
        enum Choices
        {
            WORD_CATEGORY = 0,
            AUDIO,
            CHOICES_COUNT
        };

        MenuChoices GetMenuChoices() const override
        {
            return {
                {WORD_CATEGORY, "Word Category"},
                {AUDIO, "Audio"}
            };
        }

        OptionsDropDownMenu();
        ~OptionsDropDownMenu() = default;

        void Draw() override;
        void Update(float dt) override;
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }

        using Callback = std::function<void(OptionsDropDownMenu::Choices)>;
        void AddCallback(Callback callback, OptionsDropDownMenu::Choices MenuItem);

    private:
        std::unique_ptr<RectangleDrawable> m_menuBody;
        std::unique_ptr<MenuItem> m_wordCategoryMenuItem;
        std::unique_ptr<MenuItem> m_audioMenuItem;
        Callback m_wordCategoryMenuItemCallback;
        Callback m_audioMenuItemCallback;
        void Initialize();
        void WordCategoryMenuItemClicked();
        void AudioMenuItemClicked();
        void HandleMenuItem(InputManager* InputMgr, MenuItem* MenuItem, const std::string& MenuItemName, std::function<void()> callback);
        int m_priority; // draw priority

    protected:
        void RespondToObserved(InputManager* InputMgr) override;
	};
}
