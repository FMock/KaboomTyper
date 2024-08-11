#pragma once

#include "DropDownMenu.h"

namespace GameEngine
{
    class HelpDropDownMenu : public DropDownMenu
    {
    public:
        HelpDropDownMenu();
        void ChangeMenuItemLabel(const std::string& key, const std::string& newLabel);

    protected:
        void InitializeMenuEntries() override;
        void InitializeMenuEntry(MenuItem* menuItem, const std::string& label, int x, int y, float scale, int color) override;
    };
}