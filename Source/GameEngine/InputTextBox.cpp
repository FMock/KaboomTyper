#include "InputTextBox.h"
#include <SDL.h>
#include <iostream>

using namespace GameEngine;


InputTextBox::InputTextBox()
    : m_textBox(std::make_unique<RectangleDrawable>()), m_cursor(std::make_unique<Cursor>()),
      m_cursorXPos(0), m_cursorYPos(0), m_fontWidth(24),m_initialized(false), m_full(false), 
      m_startCursorXPos(0),m_startCursorYPos(0), m_maxCharacters(0), m_isActive(false),
      m_keyPressHandler(std::make_shared<KeyPressHandler>()), m_priority(0)
{
    // Initialize the callbacks
    m_keyPressCallback = [this](std::string key) { this->OnKeyPress(key); };
    m_enterCallback = [this]() { this->OnEnter(); };
    m_backSpaceCallback = [this]() { this->OnBackspace(); };
}

InputTextBox::~InputTextBox()
{
}

GameEngine::InputTextBox::InputTextBox(int x, int y, int width, int height, Colors rectColor, bool fillWithColor) :
    m_textBox(std::make_unique<RectangleDrawable>()), m_cursor(std::make_unique<Cursor>()), m_cursorXPos(0), m_cursorYPos(0), 
    m_fontWidth(24), m_initialized(false), m_full(false), m_startCursorXPos(0), m_startCursorYPos(0), m_maxCharacters(0)
{
    InitializeTextBox(x, y, width, height);
}

void InputTextBox::InitializeTextBox(int x, int y, int width, int height, Colors rectColor, bool fillTextbox)
{
    m_textBox->Initialize(x, y, width, height, rectColor, fillTextbox);

    m_cursorXPos = x;
    m_startCursorXPos = x + 2;
    m_cursorYPos = y + 2;
    m_startCursorYPos = y + 2;
    m_maxCharacters = width / m_fontWidth;
    m_full = false;

    m_cursor->Initialize(m_startCursorXPos, m_cursorYPos, 3, height - 4, Colors::BLUE, true);

    m_initialized = true;
}

void InputTextBox::Update(float dt)
{
    m_cursor->Update(dt);
}

void InputTextBox::Draw()
{
    if (!m_initialized)
        throw std::exception("InputTextBox not initialized!\n");

    // First draw the textbox rectangle
    m_textBox->Draw();
    m_cursor->Draw();

    // Next draw the text
    const int N = m_inputText.size();
    for (int i = 0; i < N; i++)
    {
        m_inputText[i]->DrawText(1.0);
    }
}

void InputTextBox::AddText(std::string text)
{
    if (!m_full && m_cursorXPos + m_fontWidth < m_startCursorXPos + m_fontWidth + m_maxCharacters * m_fontWidth)
    {
        m_inputText.push_back(std::make_unique<TextString>(text, m_cursorXPos, m_cursorYPos));
        MoveCursorForward();
    }
}

void InputTextBox::RemoveLast()
{
    if (!m_inputText.empty())
    {
        m_inputText.pop_back();
    }
    MoveCursorBack();
}

void InputTextBox::RemoveAll()
{
    m_inputText.clear();

    MoveCursorBack();
    m_full = false;
}

void InputTextBox::AddCallback(Callback callback)
{
    m_checkforMatchCallback = callback;
}

void InputTextBox::MoveCursorForward()
{
    if (m_cursorXPos + m_fontWidth > m_startCursorXPos + m_maxCharacters * m_fontWidth)
    {
        m_cursorXPos = m_textBox->GetWidth();
        m_full = true;
    }  
    else
        m_cursorXPos += m_fontWidth;

    m_cursor->SetXPos(m_cursorXPos);
}

void InputTextBox::MoveCursorBack()
{
    if (m_cursorXPos - m_fontWidth < m_startCursorXPos)
        m_cursorXPos = m_startCursorXPos;
    else
        m_cursorXPos -= m_fontWidth;

    m_full = false;

    m_cursor->SetXPos(m_cursorXPos);
}

std::string InputTextBox::GetTextBoxContentsAsString()
{
    std::string output("");
    const int N = m_inputText.size();
    for (int i = 0; i < N; i++)
    {
        output.append(m_inputText[i]->GetText());
    }
    return output;
}

void InputTextBox::Initialize()
{
}

void InputTextBox::CheckForMatch()
{
    Common::SubmitText(GetTextBoxContentsAsString());
    m_checkforMatchCallback(); // UIManager's callback - ProcessInput()
    ClearInputText();
    SetCursorXPosition(GetCursorStartingXPosition());
    SetCursorYPosition(GetCursorStartingYPosition());
}

// Respond to key presses in the InputTextBox
void InputTextBox::RespondToObserved(InputManager* InputMgr)
{
    if (m_isActive)
    {
        // Handle key presses
        m_keyPressHandler->HandleKeyPresses(InputMgr, m_keyPressCallback, m_enterCallback, m_backSpaceCallback);
    }
}

void InputTextBox::SetCursorXPosition(int x)
{
    m_cursorXPos = x;
    m_cursor->SetXPos(m_cursorXPos);
}

void InputTextBox::SetCursorYPosition(int y)
{
    m_cursorYPos = y;
}

void InputTextBox::ClearInputText()
{
    m_inputText.clear();
}

int InputTextBox::GetCursorStartingXPosition()
{
    return m_startCursorXPos;
}

int InputTextBox::GetCursorStartingYPosition()
{
    return m_startCursorYPos;
}

void InputTextBox::SetIsActive(bool isActive)
{
    m_isActive = isActive;
}

bool InputTextBox::GetIsActive() const
{
    return m_isActive;
}

void InputTextBox::OnEnter()
{
    Common::SubmitText(GetTextBoxContentsAsString());
    CheckForMatch();
    ClearInputText();
    SetCursorXPosition(GetCursorStartingXPosition());
    SetCursorYPosition(GetCursorStartingYPosition());
}

void InputTextBox::OnBackspace()
{
    RemoveLast();
}

void InputTextBox::OnKeyPress(std::string text)
{
    AddText(text);
}
