#include "OptionsDropDownMenu.h"

using namespace GameEngine;

OptionsDropDownMenu::OptionsDropDownMenu()
{
    InitializeMenuEntries();
    InitializeMenu(125, 40, 240, 110, Colors::BLUE, true);
}

void OptionsDropDownMenu::InitializeMenuEntries()
{
    auto menuItem = std::make_unique<MenuItem>();
    if (!menuItem)
    {
        std::cerr << "Error: Failed to create Menu Item" << std::endl;
    }

    AddMenuItem("Word Category", std::move(menuItem), [this](const std::string& menuItemName)
        {
            // default is do nothing. Owners of Menu objects should call AddCallback to make the menuItem do something
        });


    AddMenuItem("Audio", std::make_unique<MenuItem>(), [this](const std::string& menuItemName)
        {

        });

}

void OptionsDropDownMenu::InitializeMenuEntry(MenuItem* menuItem, const std::string& label, int x, int y, float scale, int color)
{
    int menuXPos = 125;
    int menuYPos = 45;
    int menuWidth = 240;

    if (menuItem)
    {
        if (label == "Word Category")
        {
            menuItem->Initialize("WORD CATEGORY", menuXPos + 20, 64, menuXPos + 6, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
        }
        else if (label == "Audio")
        {
            menuItem->Initialize("AUDIO", menuXPos + 20, 110, menuXPos + 6, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
        }
    }
    else
    {
        std::cerr << "Error: Null MenuItem pointer for item: " << label << std::endl;
    }
}
