#include "InputMessageBox.h"
#include "Common.h"
#include "DrawUtils.h"
#include <SDL.h>
#include <iostream>

#define DEBUG_INPUT_MESSAGEBOX 1

using namespace DrawUtilities;
using namespace GameEngine;

InputMessageBox::InputMessageBox(int x, int y, int width, int height)
    : m_x(x), m_y(y), m_width(width), m_height(height), m_priority(0), m_focusedButton(SUBMIT), m_isActive(false), m_nextYPosition(y + 50), // Initialize next Y position with a starting offset
      m_messageBoxBody(std::make_shared<RectangleDrawable>()), m_inputTextBox(std::make_shared<InputTextBox>()), m_keyPressHandler(std::make_shared<KeyPressHandler>())
{
    m_inputManager = std::make_shared<InputManager>();
    //m_messageBoxBody = std::make_shared<RectangleDrawable>(x, y, width, height, Colors::YELLOW, true); // BROKEN. TODO: FIX
    m_messageBoxBody->Initialize(x, y, width, height, Colors::YELLOW, true);
    m_cancelButton = std::make_shared<Button>("Cancel", x + 15, y + 105, 0.7f, Colors::BLUE);
    m_submitButton = std::make_shared<Button>("Submit", x + 145, y + 105, 0.7f, Colors::BLUE);
    AddTextString(std::make_shared<TextString>("Enter Name", x + 10, y + 10));
    InitInputTextBox();

    // Initialize the callbacks
    m_keyPressCallback = [this](std::string key) { this->OnKeyPress(key); };
    m_enterCallback = [this]() { this->OnEnter(); };
    m_backSpaceCallback = [this]() { this->OnBackspace(); };
}

void InputMessageBox::Update(float dt)
{

}

void InputMessageBox::Draw()
{
    if (Common::CurrentState == GameState::IDLE && m_isActive == true)
    {
        RGBColor color = RGBColor::GetRGBColor(RGBColor::Blue);
        glDrawFilledRectangle(m_x, m_y, m_width, m_height, 1.0f, 1.0f, color);
        glDrawRectangleOutline(m_x, m_y, m_width, m_height, RGBColor::GetRGBColor(RGBColor::Red)); // red frame, consistent with menus/popups

        m_cancelButton->Draw();
        m_submitButton->Draw();
        m_inputTextBox->Draw();

        for (auto& textString : m_textStrings)
        {
            textString->DrawText(1.0f);
        }
    }
}

void InputMessageBox::InitInputTextBox()
{
    // Initialize the new InputTextBox with the calculated Y position
    m_inputTextBox->InitializeTextBox(m_x + 10, m_nextYPosition, m_width - 20, Common::FONT_HEIGHT, Colors::DEFAULT_COLOR, true);

    // Increment the next Y position by the height of the InputTextBox plus the spacing
    m_nextYPosition += Common::FONT_HEIGHT + 20;

    // Adjust the height of the InputMessageBox to fit the new InputTextBox
    int newHeight = m_nextYPosition - m_messageBoxBody->GetYPosition();
    m_messageBoxBody->SetHeight(newHeight);
}

void InputMessageBox::AddInputTextBoxCallback(Callback callback)
{
    m_enterPressedCallback = callback;
    m_inputTextBox->AddCallback(m_enterPressedCallback);
}

void InputMessageBox::AddTextString(const std::shared_ptr<TextString>& textString)
{
    m_textStrings.push_back(textString);
}

bool InputMessageBox::GetIsActive() const
{
    return m_isActive;
}

void InputMessageBox::SetIsActive(bool isActive)
{
    m_isActive = isActive;
    if (isActive)
        m_focusedButton = SUBMIT; // default focus: user can just press Enter to submit
}

void InputMessageBox::HandleButtonClick(InputManager* InputMgr, Button* button, const std::string& buttonName, std::function<void()> callback)
{
    int mouseX, mouseY;

    InputMgr->GetMousePosition(&mouseX, &mouseY);

    button->SetIsActive(button->IsMouseOverButton(mouseX, mouseY)); // toggle current button state

    if (InputMgr->m_mouseButtonState[0] && !InputMgr->m_prevMouseButtonState[0] && button->IsMouseOverButton(mouseX, mouseY))
    {
#if DEBUG_INPUT_MESSAGEBOX
        std::cout << buttonName << " button clicked" << std::endl;
#endif
        button->SetButtonColor(Colors::DARK_GRAY);
        Common::SubmitText(m_inputTextBox->GetTextBoxContentsAsString());
        callback();
    }
    else if (!InputMgr->m_mouseButtonState[0] && InputMgr->m_prevMouseButtonState[0] && button->IsMouseOverButton(mouseX, mouseY))
    {
#if DEBUG_INPUT_MESSAGEBOX
        std::cout << buttonName << " button released" << std::endl;
#endif
        button->SetButtonColor(Colors::DEFAULT_COLOR);
    }
}

void InputMessageBox::AddButtonCallback(Callback callback, InputMessageBox::Buttons buttonName)
{
    switch (buttonName)
    {
    case GameEngine::InputMessageBox::CANCEL:
        m_cancelBtnCallback = callback;
        break;
    case GameEngine::InputMessageBox::SUBMIT:
        m_submitBtnCallback = callback;
        break;
    default:
        break;
    }
}

void InputMessageBox::OnEnter()
{
    // Enter activates whichever button has keyboard focus.
    if (m_focusedButton == CANCEL)
    {
        if (m_cancelBtnCallback)
            m_cancelBtnCallback();
    }
    else // SUBMIT
    {
        Common::SubmitText(m_inputTextBox->GetTextBoxContentsAsString());
        if (m_enterPressedCallback)
            m_enterPressedCallback();
    }
    m_inputTextBox->ClearInputText();
    m_inputTextBox->SetCursorXPosition(m_inputTextBox->GetCursorStartingXPosition());
    m_inputTextBox->SetCursorYPosition(m_inputTextBox->GetCursorStartingYPosition());
}

void InputMessageBox::OnBackspace()
{
    m_inputTextBox->RemoveLast();
}

void InputMessageBox::OnKeyPress(std::string text)
{
    m_inputTextBox->AddText(text);
}

void InputMessageBox::RespondToObserved(InputManager* InputMgr)
{
    if (m_isActive)
    {
        // Left/Right (and Up/Down) move keyboard focus between Submit and Cancel.
        auto justPressed = [InputMgr](SDL_Scancode sc)
        {
            return InputMgr->m_kbState[sc] && !InputMgr->m_kbPrevState[sc];
        };
        if (justPressed(SDL_SCANCODE_LEFT) || justPressed(SDL_SCANCODE_RIGHT) ||
            justPressed(SDL_SCANCODE_UP) || justPressed(SDL_SCANCODE_DOWN))
        {
            m_focusedButton = (m_focusedButton == SUBMIT) ? CANCEL : SUBMIT;
        }

        // Show the focused button as a persistent highlight.
        m_submitButton->SetSelected(m_focusedButton == SUBMIT);
        m_cancelButton->SetSelected(m_focusedButton == CANCEL);

        // Handle button clicks
        HandleButtonClick(InputMgr, m_cancelButton.get(), "Cancel", [this]() { if (m_cancelBtnCallback) m_cancelBtnCallback(); });
        HandleButtonClick(InputMgr, m_submitButton.get(), "Submit", [this]() { if (m_submitBtnCallback) m_submitBtnCallback(); });

        // Pass the callbacks to HandleKeyPresses
        m_keyPressHandler->HandleKeyPresses(InputMgr, m_keyPressCallback, m_enterCallback, m_backSpaceCallback);
    }
}
