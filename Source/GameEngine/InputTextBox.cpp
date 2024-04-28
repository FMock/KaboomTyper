#include "InputTextBox.h"
#include <SDL.h>
#include <iostream>

using namespace GameEngine;

InputTextBox::InputTextBox() : m_textBox(std::make_unique<RectangleDrawable>()), m_cursorXPos(0), m_cursorYPos(0), m_fontWidth(24),m_initialized(false)
{
}

InputTextBox::~InputTextBox()
{
}

void InputTextBox::InitializeTextBox(int x, int y, int width, int height, Colors rectColor)
{
    m_textBox->Initialize(x, y, width, height, rectColor);
    m_cursorXPos = x;
    m_cursorYPos = y + 2;
    m_initialized = true;
}

void InputTextBox::Update()
{
}

void InputTextBox::Draw()
{
    if (!m_initialized)
        throw std::exception("InputTextBox not initialized!\n");

    // First draw the textbox rectangle
    m_textBox->Draw();

    // Next draw the text
    for (int i = 0; i < m_inputText.size(); i++)
    {
        m_inputText[i]->DrawText();
    }
}

void InputTextBox::AddText(std::string text)
{
    m_inputText.push_back(std::make_unique<TextString>(text, m_cursorXPos, m_cursorYPos));
    MoveCursorForward();
}

void InputTextBox::RemoveLast()
{
    if (!m_inputText.empty())
    {
        m_inputText.pop_back();
    }
    MoveCursorBack();
}

void InputTextBox::MoveCursorForward()
{
    m_cursorXPos += m_fontWidth;
}

void InputTextBox::MoveCursorBack()
{
    m_cursorXPos -= m_fontWidth;
}

void InputTextBox::Initialize()
{
}

// Respond to key presses in the InputTextBox
void InputTextBox::RespondToObserved(InputManager* InputMgr)
{
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
        if (!InputMgr->m_kbPrevState[i] && InputMgr->m_kbState[i])
        {
            switch (i)
            {
            case SDL_SCANCODE_A:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("A", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("a", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_B:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("B", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("b", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_C:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("C", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("c", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_D:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("D", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("d", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_E:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("E", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("e", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_F:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("F", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("f", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_G:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("G", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("g", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_H:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("H", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("h", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_I:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("I", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("i", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_J:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("J", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("j", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_K:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("K", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("k", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_L:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("L", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("l", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_M:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("M", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("m", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_N:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("N", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("n", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_O:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("O", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("o", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_P:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("P", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("p", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_Q:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("Q", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("q", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_R:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("R", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("r", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_S:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("S", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("s", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_T:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("T", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("t", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_U:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("U", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("u", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_V:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("V", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("v", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_W:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("W", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("w", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_X:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("X", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("x", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_Y:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("Y", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("y", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_Z:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("Z", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("z", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_0:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>(")", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("0", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_1:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("!", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("1", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_2:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("@", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("2", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_3:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("#", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("3", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_4:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("$", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("4", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_5:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("%", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("5", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_6:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("^", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("6", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_7:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("&", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("7", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_8:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("*", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("8", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_9:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("(", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("9", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_MINUS:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("_", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("-", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_EQUALS:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("+", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("=", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_COMMA:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("<", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>(",", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_PERIOD:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>(">", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>(".", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_SLASH:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>("?", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("/", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_SEMICOLON:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>(":", m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>(";", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_APOSTROPHE:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>(std::string("\""), m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("'", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_SPACE:
                m_inputText.push_back(std::make_unique<TextString>(std::string(" "), m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_LEFTBRACKET:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>(std::string("\{"), m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("[", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_RIGHTBRACKET:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>(std::string("\}"), m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>("]", m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_BACKSLASH:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>(std::string("\|"), m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>(std::string("\\"), m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_GRAVE:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    m_inputText.push_back(std::make_unique<TextString>(std::string("\~"), m_cursorXPos, m_cursorYPos));
                else
                    m_inputText.push_back(std::make_unique<TextString>(std::string("\`"), m_cursorXPos, m_cursorYPos));
                MoveCursorForward();
                break;
            case SDL_SCANCODE_F1:
                //m_inputText.push_back(std::make_unique<TextString>(std::string("F1"), m_cursorXPos, m_cursorYPos));
                //MoveCursorForward();
                break;
            case SDL_SCANCODE_F2:
                //m_inputText.push_back(std::make_unique<TextString>(std::string("F2"), m_cursorXPos, m_cursorYPos));
                //MoveCursorForward();
                break;
            case SDL_SCANCODE_F3:
                //m_inputText.push_back(std::make_unique<TextString>(std::string("F3"), m_cursorXPos, m_cursorYPos));
                //MoveCursorForward();
                break;
            case SDL_SCANCODE_F4:
                //m_inputText.push_back(std::make_unique<TextString>(std::string("F4"), m_cursorXPos, m_cursorYPos));
                //MoveCursorForward();
                break;
            case SDL_SCANCODE_BACKSPACE:
                RemoveLast();
                break;
                // Add
            default:
                // do nothing
                break;
            }
        }
    }
}
