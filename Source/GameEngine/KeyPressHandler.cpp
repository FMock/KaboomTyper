#include "KeyPressHandler.h"

using namespace GameEngine;

void KeyPressHandler::HandleKeyPresses(InputManager* InputMgr, KeyPressCallback keyPressCallback, EnterKeyCallback enterKeyCallback, BackSpaceCallback backSpaceCallback)
{
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
        if (!InputMgr->m_kbPrevState[i] && InputMgr->m_kbState[i])
        {
            auto shiftPressed = InputMgr->m_kbState[SDL_SCANCODE_LSHIFT] || InputMgr->m_kbState[SDL_SCANCODE_RSHIFT];
            switch (i)
            {
            case SDL_SCANCODE_A: keyPressCallback(shiftPressed ? "A" : "a"); break;
            case SDL_SCANCODE_B: keyPressCallback(shiftPressed ? "B" : "b"); break;
            case SDL_SCANCODE_C: keyPressCallback(shiftPressed ? "C" : "c"); break;
            case SDL_SCANCODE_D: keyPressCallback(shiftPressed ? "D" : "d"); break;
            case SDL_SCANCODE_E: keyPressCallback(shiftPressed ? "E" : "e"); break;
            case SDL_SCANCODE_F: keyPressCallback(shiftPressed ? "F" : "f"); break;
            case SDL_SCANCODE_G: keyPressCallback(shiftPressed ? "G" : "g"); break;
            case SDL_SCANCODE_H: keyPressCallback(shiftPressed ? "H" : "h"); break;
            case SDL_SCANCODE_I: keyPressCallback(shiftPressed ? "I" : "i"); break;
            case SDL_SCANCODE_J: keyPressCallback(shiftPressed ? "J" : "j"); break;
            case SDL_SCANCODE_K: keyPressCallback(shiftPressed ? "K" : "k"); break;
            case SDL_SCANCODE_L: keyPressCallback(shiftPressed ? "L" : "l"); break;
            case SDL_SCANCODE_M: keyPressCallback(shiftPressed ? "M" : "m"); break;
            case SDL_SCANCODE_N: keyPressCallback(shiftPressed ? "N" : "n"); break;
            case SDL_SCANCODE_O: keyPressCallback(shiftPressed ? "O" : "o"); break;
            case SDL_SCANCODE_P: keyPressCallback(shiftPressed ? "P" : "p"); break;
            case SDL_SCANCODE_Q: keyPressCallback(shiftPressed ? "Q" : "q"); break;
            case SDL_SCANCODE_R: keyPressCallback(shiftPressed ? "R" : "r"); break;
            case SDL_SCANCODE_S: keyPressCallback(shiftPressed ? "S" : "s"); break;
            case SDL_SCANCODE_T: keyPressCallback(shiftPressed ? "T" : "t"); break;
            case SDL_SCANCODE_U: keyPressCallback(shiftPressed ? "U" : "u"); break;
            case SDL_SCANCODE_V: keyPressCallback(shiftPressed ? "V" : "v"); break;
            case SDL_SCANCODE_W: keyPressCallback(shiftPressed ? "W" : "w"); break;
            case SDL_SCANCODE_X: keyPressCallback(shiftPressed ? "X" : "x"); break;
            case SDL_SCANCODE_Y: keyPressCallback(shiftPressed ? "Y" : "y"); break;
            case SDL_SCANCODE_Z: keyPressCallback(shiftPressed ? "Z" : "z"); break;
            case SDL_SCANCODE_0: keyPressCallback(shiftPressed ? ")" : "0"); break;
            case SDL_SCANCODE_1: keyPressCallback(shiftPressed ? "!" : "1"); break;
            case SDL_SCANCODE_2: keyPressCallback(shiftPressed ? "@" : "2"); break;
            case SDL_SCANCODE_3: keyPressCallback(shiftPressed ? "#" : "3"); break;
            case SDL_SCANCODE_4: keyPressCallback(shiftPressed ? "$" : "4"); break;
            case SDL_SCANCODE_5: keyPressCallback(shiftPressed ? "%" : "5"); break;
            case SDL_SCANCODE_6: keyPressCallback(shiftPressed ? "^" : "6"); break;
            case SDL_SCANCODE_7: keyPressCallback(shiftPressed ? "&" : "7"); break;
            case SDL_SCANCODE_8: keyPressCallback(shiftPressed ? "*" : "8"); break;
            case SDL_SCANCODE_9: keyPressCallback(shiftPressed ? "(" : "9"); break;
            case SDL_SCANCODE_MINUS: keyPressCallback(shiftPressed ? "_" : "-"); break;
            case SDL_SCANCODE_EQUALS: keyPressCallback(shiftPressed ? "+" : "="); break;
            case SDL_SCANCODE_COMMA: keyPressCallback(shiftPressed ? "<" : ","); break;
            case SDL_SCANCODE_PERIOD: keyPressCallback(shiftPressed ? ">" : "."); break;
            case SDL_SCANCODE_SLASH: keyPressCallback(shiftPressed ? "?" : "/"); break;
            case SDL_SCANCODE_SEMICOLON: keyPressCallback(shiftPressed ? ":" : ";"); break;
            case SDL_SCANCODE_APOSTROPHE: keyPressCallback(shiftPressed ? "\"" : "'"); break;
            case SDL_SCANCODE_SPACE: keyPressCallback(" "); break;
            case SDL_SCANCODE_LEFTBRACKET: keyPressCallback(shiftPressed ? "{" : "["); break;
            case SDL_SCANCODE_RIGHTBRACKET: keyPressCallback(shiftPressed ? "}" : "]"); break;
            case SDL_SCANCODE_BACKSLASH: keyPressCallback(shiftPressed ? "|" : "\\"); break;
            case SDL_SCANCODE_GRAVE: keyPressCallback(shiftPressed ? "~" : "`"); break;
            case SDL_SCANCODE_BACKSPACE: backSpaceCallback(); break;
            case SDL_SCANCODE_RETURN: enterKeyCallback(); break;
            default: break; // do nothing
            }
        }
    }
}
