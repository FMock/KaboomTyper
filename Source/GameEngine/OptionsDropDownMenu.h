#pragma once

#include "DropDownMenu.h"

namespace GameEngine
{
    class OptionsDropDownMenu : public DropDownMenu
    {
    public:
        OptionsDropDownMenu();

    protected:
        void InitializeMenuEntries() override;
        void InitializeMenuEntry(MenuItem* menuItem, const std::string& label, int x, int y, float scale, int color) override;
    };
}