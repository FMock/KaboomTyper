#pragma once

#include "ContextMenu.h"
#include "RectangleDrawable.h"
//#include "MenuItem.h"
#include "MenuItem.h"
#include "InputManager.h"
#include <memory>
#include <functional>

namespace GameEngine
{
    class FileContextMenu : public ContextMenu
    {
    public:
        enum Choices
        {
            IMPORT = 0,
            EXIT
        };

        MenuChoices GetMenuChoices() const override
        {
            return {
                {IMPORT, "Import"},
                {EXIT, "Exit"}
            };
        }

        FileContextMenu();
        ~FileContextMenu() = default;

        void Draw();
        void Update();
        bool GetIsActive() const;
        void SetIsActive(bool);

        using Callback = std::function<void(FileContextMenu::Choices)>;
        void AddCallback(Callback callback, FileContextMenu::Choices MenuItem);

    private:
        std::unique_ptr<RectangleDrawable> m_menuBody;
        std::unique_ptr<MenuItem> m_importBtn;
        std::unique_ptr<MenuItem> m_exitBtn;
        void Initialize();
        Callback m_importBtnCallback;
        void ImportMenuItemClicked();
        Callback m_exitBtnCallback;
        void ExitMenuItemClicked();
        void HandleMenuItem(InputManager* InputMgr, MenuItem* MenuItem, const std::string& MenuItemName, std::function<void()> callback);

    protected:
        void RespondToObserved(InputManager* InputMgr) override;
    };
}