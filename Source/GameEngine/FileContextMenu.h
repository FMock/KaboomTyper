#pragma once

#include "ContextMenu.h"
#include "RectangleDrawable.h"
#include "Button.h"
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
        void AddCallback(Callback callback, FileContextMenu::Choices button);

    private:
        std::unique_ptr<RectangleDrawable> m_menuBody;
        std::unique_ptr<Button> m_importBtn;
        std::unique_ptr<Button> m_exitBtn;
        void Initialize();
        Callback m_importBtnCallback;
        void ImportButtonClicked();
        Callback m_exitBtnCallback;
        void ExitButtonClicked();
        void HandleButton(InputManager* InputMgr, Button* button, const std::string& buttonName, std::function<void()> callback);

    protected:
        void RespondToObserved(InputManager* InputMgr) override;
    };
}