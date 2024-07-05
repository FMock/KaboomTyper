/*
* The main menu at the top the the game window
*/
#pragma once

#include "Menu.h"

namespace GameEngine
{
    class MainMenu : public Menu
    {
    public:
        MainMenu();
    protected:
        void InitializeMenuItems() override;
        void InitializeMenuItem(Button* button, const std::string& label, int x, int y, float scale, int color) override;
    };
}
