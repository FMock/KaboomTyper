#include "InputMessageBox.h"
#include "Common.h"
#include "DrawUtils.h"
#include <iostream>

#define DEBUG_INPUT_MESSAGEBOX 1

using namespace DrawUtilities;
using namespace GameEngine;

InputMessageBox::InputMessageBox(int x, int y, int width, int height)
    : m_x(x), m_y(y), m_width(width), m_height(height), m_priority(0), m_isActive(false), m_nextYPosition(y + 50) // Initialize next Y position with a starting offset
{
    m_inputManager = std::make_shared<InputManager>();
    //m_messageBoxBody = std::make_shared<RectangleDrawable>(x, y, width, height, Colors::YELLOW, true); // BROKEN. TODO: FIX
    m_messageBoxBody = std::make_shared<RectangleDrawable>();
    m_messageBoxBody->Initialize(x, y, width, height, Colors::YELLOW, true);
    m_cancelButton = std::make_shared<Button>("Cancel", x + 15, y + 105, 0.7f, Colors::BLUE);
    m_submitButton = std::make_shared<Button>("Submit", x + 145, y + 105, 0.7f, Colors::BLUE);
    AddTextString(std::make_shared<TextString>("Enter Name", x + 10, y + 10));

}

void InputMessageBox::Update(float dt)
{
    for (auto& inputTextBox : m_inputTextBoxes)
    {
        // Update each InputTextBox
    }

    for (auto& textString : m_textStrings)
    {
        // Update each TextString
    }
}

void InputMessageBox::Draw()
{
    if (Common::CurrentState == GameState::IDLE && m_isActive == true)
    {
        RGBColor color = RGBColor::GetRGBColor(RGBColor::Blue);
        glDrawFilledRectangle(m_x, m_y, m_width, m_height, 1.0f, 1.0f, color);

        m_cancelButton->Draw();
        m_submitButton->Draw();


        for (auto& inputTextBox : m_inputTextBoxes)
        {
            inputTextBox->Draw();
        }

        for (auto& textString : m_textStrings)
        {
            textString->DrawText(1.0f);
        }
    }
}

void InputMessageBox::AddInputTextBox(Callback callback)
{
    m_callback = callback;

    auto inputTextBox = std::make_shared<InputTextBox>();

    int yPosition = m_nextYPosition;

    // Initialize the new InputTextBox with the calculated Y position
    inputTextBox->InitializeTextBox(m_x + 10, yPosition, m_width - 20, Common::FONT_HEIGHT, Colors::DEFAULT_COLOR, true);

    // Increment the next Y position by the height of the InputTextBox plus the spacing
    m_nextYPosition += Common::FONT_HEIGHT + 20;

    // Adjust the height of the InputMessageBox to fit the new InputTextBox
    int newHeight = m_nextYPosition - m_messageBoxBody->GetYPosition();
    m_messageBoxBody->SetHeight(newHeight);

    inputTextBox->AddCallback(m_callback);

    m_inputTextBoxes.push_back(inputTextBox);
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
}

void InputMessageBox::RespondToObserved(InputManager* InputMgr)
{
    if (m_isActive)
    {
        for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
        {
            if (!InputMgr->m_kbPrevState[i] && InputMgr->m_kbState[i])
            {
                switch (i)
                {
                case SDL_SCANCODE_A:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("A");
                    else
                        m_inputTextBoxes[0]->AddText("a");
                    break;
                case SDL_SCANCODE_B:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("B");
                    else
                        m_inputTextBoxes[0]->AddText("b");
                    break;
                case SDL_SCANCODE_C:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("C");
                    else
                        m_inputTextBoxes[0]->AddText("c");
                    break;
                case SDL_SCANCODE_D:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("D");
                    else
                        m_inputTextBoxes[0]->AddText("d");
                    break;
                case SDL_SCANCODE_E:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("E");
                    else
                        m_inputTextBoxes[0]->AddText("e");
                    break;
                case SDL_SCANCODE_F:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("F");
                    else
                        m_inputTextBoxes[0]->AddText("f");
                    break;
                case SDL_SCANCODE_G:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("G");
                    else
                        m_inputTextBoxes[0]->AddText("g");
                    break;
                case SDL_SCANCODE_H:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("H");
                    else
                        m_inputTextBoxes[0]->AddText("h");
                    break;
                case SDL_SCANCODE_I:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("I");
                    else
                        m_inputTextBoxes[0]->AddText("i");
                    break;
                case SDL_SCANCODE_J:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("J");
                    else
                        m_inputTextBoxes[0]->AddText("j");
                    break;
                case SDL_SCANCODE_K:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("K");
                    else
                        m_inputTextBoxes[0]->AddText("k");
                    break;
                case SDL_SCANCODE_L:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("L");
                    else
                        m_inputTextBoxes[0]->AddText("l");
                    break;
                case SDL_SCANCODE_M:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("M");
                    else
                        m_inputTextBoxes[0]->AddText("m");
                    break;
                case SDL_SCANCODE_N:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("N");
                    else
                        m_inputTextBoxes[0]->AddText("n");
                    break;
                case SDL_SCANCODE_O:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("O");
                    else
                        m_inputTextBoxes[0]->AddText("o");
                    break;
                case SDL_SCANCODE_P:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("P");
                    else
                        m_inputTextBoxes[0]->AddText("p");
                    break;
                case SDL_SCANCODE_Q:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("Q");
                    else
                        m_inputTextBoxes[0]->AddText("q");
                    break;
                case SDL_SCANCODE_R:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("R");
                    else
                        m_inputTextBoxes[0]->AddText("r");
                    break;
                case SDL_SCANCODE_S:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("S");
                    else
                        m_inputTextBoxes[0]->AddText("s");
                    break;
                case SDL_SCANCODE_T:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("T");
                    else
                        m_inputTextBoxes[0]->AddText("t");
                    break;
                case SDL_SCANCODE_U:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("U");
                    else
                        m_inputTextBoxes[0]->AddText("u");
                    break;
                case SDL_SCANCODE_V:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("V");
                    else
                        m_inputTextBoxes[0]->AddText("v");
                    break;
                case SDL_SCANCODE_W:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("W");
                    else
                        m_inputTextBoxes[0]->AddText("w");
                    break;
                case SDL_SCANCODE_X:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("X");
                    else
                        m_inputTextBoxes[0]->AddText("x");
                    break;
                case SDL_SCANCODE_Y:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("Y");
                    else
                        m_inputTextBoxes[0]->AddText("y");
                    break;
                case SDL_SCANCODE_Z:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("Z");
                    else
                        m_inputTextBoxes[0]->AddText("z");
                    break;
                case SDL_SCANCODE_0:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText(")");
                    else
                        m_inputTextBoxes[0]->AddText("0");
                    break;
                case SDL_SCANCODE_1:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("!");
                    else
                        m_inputTextBoxes[0]->AddText("1");
                    break;
                case SDL_SCANCODE_2:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("@");
                    else
                        m_inputTextBoxes[0]->AddText("2");
                    break;
                case SDL_SCANCODE_3:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("#");
                    else
                        m_inputTextBoxes[0]->AddText("3");
                    break;
                case SDL_SCANCODE_4:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("$");
                    else
                        m_inputTextBoxes[0]->AddText("4");
                    break;
                case SDL_SCANCODE_5:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("%");
                    else
                        m_inputTextBoxes[0]->AddText("5");
                    break;
                case SDL_SCANCODE_6:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("^");
                    else
                        m_inputTextBoxes[0]->AddText("6");
                    break;
                case SDL_SCANCODE_7:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("&");
                    else
                        m_inputTextBoxes[0]->AddText("7");
                    break;
                case SDL_SCANCODE_8:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("*");
                    else
                        m_inputTextBoxes[0]->AddText("8");
                    break;
                case SDL_SCANCODE_9:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("(");
                    else
                        m_inputTextBoxes[0]->AddText("9");
                    break;
                case SDL_SCANCODE_MINUS:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("_");
                    else
                        m_inputTextBoxes[0]->AddText("-");
                    break;
                case SDL_SCANCODE_EQUALS:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("+");
                    else
                        m_inputTextBoxes[0]->AddText("=");
                    break;
                case SDL_SCANCODE_COMMA:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("<");
                    else
                        m_inputTextBoxes[0]->AddText(",");
                    break;
                case SDL_SCANCODE_PERIOD:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText(">");
                    else
                        m_inputTextBoxes[0]->AddText(".");
                    break;
                case SDL_SCANCODE_SLASH:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("?");
                    else
                        m_inputTextBoxes[0]->AddText("/");
                    break;
                case SDL_SCANCODE_SEMICOLON:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText(":");
                    else
                        m_inputTextBoxes[0]->AddText(";");
                    break;
                case SDL_SCANCODE_APOSTROPHE:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText("\"");
                    else
                        m_inputTextBoxes[0]->AddText("'");
                    break;
                case SDL_SCANCODE_SPACE:
                    m_inputTextBoxes[0]->AddText(std::string(" "));
                    break;
                case SDL_SCANCODE_LEFTBRACKET:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText(std::string("\{"));
                    else
                        m_inputTextBoxes[0]->AddText("[");
                    break;
                case SDL_SCANCODE_RIGHTBRACKET:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText(std::string("\}"));
                    else
                        m_inputTextBoxes[0]->AddText("]");
                    break;
                case SDL_SCANCODE_BACKSLASH:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText(std::string("|"));
                    else
                        m_inputTextBoxes[0]->AddText(std::string("\\"));
                    break;
                case SDL_SCANCODE_GRAVE:
                    if (InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT])
                        m_inputTextBoxes[0]->AddText(std::string("~"));
                    else
                        m_inputTextBoxes[0]->AddText(std::string("`"));
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
                    m_inputTextBoxes[0]->RemoveLast();
                    break;
                case SDL_SCANCODE_RETURN:
                    Common::SubmitText(m_inputTextBoxes[0]->GetTextBoxContentsAsString());
                    m_callback();
                    m_inputTextBoxes[0]->ClearInputText();
                    m_inputTextBoxes[0]->SetCursorXPosition(m_inputTextBoxes[0]->GetCursorStartingXPosition());
                    m_inputTextBoxes[0]->SetCursorYPosition(m_inputTextBoxes[0]->GetCursorStartingYPosition());

                    break;
                default:
                    // do nothing
                    break;
                }
            }
        }
    }
}
