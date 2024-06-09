#include "InputTextBox.h"
#include <SDL.h>
#include <iostream>

using namespace GameEngine;


InputTextBox::InputTextBox()
    : m_textBox(std::make_unique<RectangleDrawable>()), m_cursor(std::make_unique<Cursor>()), m_cursorXPos(0), m_cursorYPos(0), m_fontWidth(24),m_initialized(false), m_full(false), m_startCursorXPos(0),m_startCursorYPos(0), m_maxCharacters(0)
{
}

InputTextBox::~InputTextBox()
{
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

    m_cursor->Initialize(m_startCursorXPos, m_cursorYPos, 3, height - 4, Colors::DARK_GRAY, true);

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
    m_checkforMatchCallback(); // GameManagers callback that comapares submitted and active string
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
                    AddText("A");
                else
                    AddText("a");
                break;
            case SDL_SCANCODE_B:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("B");
                else
                    AddText("b");
                break;
            case SDL_SCANCODE_C:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("C");
                else
                    AddText("c");
                break;
            case SDL_SCANCODE_D:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("D");
                else
                    AddText("d");
                break;
            case SDL_SCANCODE_E:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("E");
                else
                    AddText("e");
                break;
            case SDL_SCANCODE_F:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("F");
                else
                    AddText("f");
                break;
            case SDL_SCANCODE_G:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("G");
                else
                    AddText("g");
                break;
            case SDL_SCANCODE_H:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("H");
                else
                    AddText("h");
                break;
            case SDL_SCANCODE_I:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("I");
                else
                    AddText("i");
                break;
            case SDL_SCANCODE_J:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("J");
                else
                    AddText("j");
                break;
            case SDL_SCANCODE_K:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("K");
                else
                    AddText("k");
                break;
            case SDL_SCANCODE_L:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("L");
                else
                    AddText("l");
                break;
            case SDL_SCANCODE_M:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("M");
                else
                    AddText("m");
                break;
            case SDL_SCANCODE_N:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("N");
                else
                    AddText("n");
                break;
            case SDL_SCANCODE_O:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("O");
                else
                    AddText("o");
                break;
            case SDL_SCANCODE_P:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("P");
                else
                    AddText("p");
                break;
            case SDL_SCANCODE_Q:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("Q");
                else
                    AddText("q");
                break;
            case SDL_SCANCODE_R:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("R");
                else
                    AddText("r");
                break;
            case SDL_SCANCODE_S:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("S");
                else
                    AddText("s");
                break;
            case SDL_SCANCODE_T:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("T");
                else
                    AddText("t");
                break;
            case SDL_SCANCODE_U:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("U");
                else
                    AddText("u");
                break;
            case SDL_SCANCODE_V:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("V");
                else
                    AddText("v");
                break;
            case SDL_SCANCODE_W:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("W");
                else
                    AddText("w");
                break;
            case SDL_SCANCODE_X:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("X");
                else
                    AddText("x");
                break;
            case SDL_SCANCODE_Y:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("Y");
                else
                    AddText("y");
                break;
            case SDL_SCANCODE_Z:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("Z");
                else
                    AddText("z");
                break;
            case SDL_SCANCODE_0:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText(")");
                else
                    AddText("0");
                break;
            case SDL_SCANCODE_1:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("!");
                else
                    AddText("1");
                break;
            case SDL_SCANCODE_2:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("@");
                else
                    AddText("2");
                break;
            case SDL_SCANCODE_3:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("#");
                else
                    AddText("3");
                break;
            case SDL_SCANCODE_4:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("$");
                else
                    AddText("4");
                break;
            case SDL_SCANCODE_5:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("%");
                else
                    AddText("5");
                break;
            case SDL_SCANCODE_6:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("^");
                else
                    AddText("6");
                break;
            case SDL_SCANCODE_7:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("&");
                else
                    AddText("7");
                break;
            case SDL_SCANCODE_8:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("*");
                else
                    AddText("8");
                break;
            case SDL_SCANCODE_9:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("(");
                else
                    AddText("9");
                break;
            case SDL_SCANCODE_MINUS:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("_");
                else
                    AddText("-");
                break;
            case SDL_SCANCODE_EQUALS:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("+");
                else
                    AddText("=");
                break;
            case SDL_SCANCODE_COMMA:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("<");
                else
                    AddText(",");
                break;
            case SDL_SCANCODE_PERIOD:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText(">");
                else
                    AddText(".");
                break;
            case SDL_SCANCODE_SLASH:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("?");
                else
                    AddText("/");
                break;
            case SDL_SCANCODE_SEMICOLON:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText(":");
                else
                    AddText(";");
                break;
            case SDL_SCANCODE_APOSTROPHE:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText("\"");
                else
                    AddText("'");
                break;
            case SDL_SCANCODE_SPACE:
                AddText(std::string(" "));
                break;
            case SDL_SCANCODE_LEFTBRACKET:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText(std::string("\{"));
                else
                    AddText("[");
                break;
            case SDL_SCANCODE_RIGHTBRACKET:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText(std::string("\}"));
                else
                    AddText("]");
                break;
            case SDL_SCANCODE_BACKSLASH:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText(std::string("|"));
                else
                    AddText(std::string("\\"));
                break;
            case SDL_SCANCODE_GRAVE:
                if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                    AddText(std::string("~"));
                else
                    AddText(std::string("`"));
                break;
            case SDL_SCANCODE_F1:
                if (InputMgr->m_kbState[SDL_SCANCODE_F1])
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
            case SDL_SCANCODE_RETURN:
                Common::SubmitText(GetTextBoxContentsAsString());
                CheckForMatch();
                m_inputText.clear();
                m_cursorXPos = m_startCursorXPos;
                m_cursorYPos = m_startCursorYPos;
                m_cursor->SetXPos(m_cursorXPos);
                break;
            default:
                // do nothing
                break;
            }
        }
    }
}
