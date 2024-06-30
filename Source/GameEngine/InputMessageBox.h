#pragma once

#include "IInputMessageBox.h"
#include "InputTextBox.h"
#include "RectangleDrawable.h"
#include "TextString.h"
#include "Button.h"
#include "KeyPressHandler.h"
#include <memory>
#include <vector>
#include <string>
#include <functional>

namespace GameEngine
{
    class InputMessageBox : public IInputMessageBox, public InputObserver, public IDrawable
    {
    public:
        InputMessageBox(int x = 250, int y = 300, int width = 260, int height = 150);

        enum Buttons
        {
            CANCEL = 0,
            SUBMIT
        };

        void Update(float dt) override;
        void Draw() override;
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }

        using Callback = std::function<void()>;
        void AddTextString(const std::shared_ptr<TextString>& textString);
        bool GetIsActive() const;
        void SetIsActive(bool isActive);
        void AddButtonCallback(Callback callback, InputMessageBox::Buttons buttonName);
        void AddInputTextBoxCallback(Callback callback);

    protected:
        void RespondToObserved(InputManager* InputMgr) override;

    private:
        void InitInputTextBox();
        void HandleButtonClick(InputManager* InputMgr, Button* button, const std::string& buttonName, std::function<void()> callback);
        Callback m_enterPressedCallback;
        Callback m_cancelBtnCallback;
        Callback m_submitBtnCallback;
        std::shared_ptr<RectangleDrawable> m_messageBoxBody;
        std::shared_ptr<InputTextBox> m_inputTextBox;
        std::vector<std::unique_ptr<TextString>> m_inputText;
        std::vector<std::shared_ptr<TextString>> m_textStrings;
        std::shared_ptr<Button> m_cancelButton;
        std::shared_ptr<Button> m_submitButton;
        bool m_isActive;
        int m_nextYPosition; // To keep track of the next Y position for InputTextBox
        int m_priority; // draw priority
        int m_x, m_y, m_width, m_height;
        std::shared_ptr<KeyPressHandler> m_keyPressHandler;
        KeyPressHandler::KeyPressCallback m_keyPressCallback;
        KeyPressHandler::EnterKeyCallback m_enterCallback;
        KeyPressHandler::BackSpaceCallback m_backSpaceCallback;
        void OnEnter();
        void OnBackspace();
        void OnKeyPress(std::string);
    };
}
