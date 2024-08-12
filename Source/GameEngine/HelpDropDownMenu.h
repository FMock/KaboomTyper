#pragma once

#include "DropDownMenu.h"

namespace GameEngine
{
    class HelpDropDownMenu : public DropDownMenu
    {
    public:
        HelpDropDownMenu();
        void ChangeMenuItemLabel(const std::string& key, const std::string& newLabel);

    private:
        const int X_POSITION = 270;
        const int Y_POSITION = 40;
        const int WIDTH = 300;
        const int HEIGHT = 170;

    protected:
        void InitializeMenuEntries() override;
        void InitializeMenuEntry(MenuItem* menuItem, const std::string& label, int x, int y, float scale, int color) override;
    };
}