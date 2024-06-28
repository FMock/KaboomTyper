#pragma once

#include "IInputMessageBox.h"
#include "InputTextBox.h"
#include "RectangleDrawable.h"
#include "TextString.h"
#include "Button.h"
#include <memory>
#include <vector>
#include <functional>

namespace GameEngine
{
    class InputMessageBox : public IInputMessageBox, public InputObserver, public IDrawable
    {
    public:
        InputMessageBox(int x = 250, int y = 300, int width = 260, int height = 150);

        void Update(float dt) override;
        void Draw() override;
        int GetPriority() const override { return m_priority; }
        void SetPriority(int priority) override { m_priority = priority; }

        using Callback = std::function<void()>;

        void AddInputTextBox(Callback);
        void AddTextString(const std::shared_ptr<TextString>& textString);
        bool GetIsActive() const;
        void SetIsActive(bool isActive);

    protected:
        void RespondToObserved(InputManager* InputMgr) override;

    private:
        Callback m_callback;
        int m_nextYPosition; // To keep track of the next Y position for InputTextBox
        std::shared_ptr<RectangleDrawable> m_messageBoxBody;
        std::vector<std::shared_ptr<InputTextBox>> m_inputTextBoxes;
        std::vector<std::shared_ptr<TextString>> m_textStrings;
        std::shared_ptr<Button> m_cancelButton;
        std::shared_ptr<Button> m_submitButton;
        bool m_isActive;
        int m_priority; // draw priority
        int m_x, m_y, m_width, m_height;
    };
}
