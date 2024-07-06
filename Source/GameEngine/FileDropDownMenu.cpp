#include "FileDropDownMenu.h"

GameEngine::FileDropDownMenu::FileDropDownMenu()
{
	InitializeMenuEntries();
	InitializeMenu(10, 40, 200, 110, Colors::BLUE, true);
}

void GameEngine::FileDropDownMenu::InitializeMenuEntries()
{
    auto menuItem = std::make_unique<MenuItem>();
    if (!menuItem)
    {
        std::cerr << "Error: Failed to create Menu Item" << std::endl;
    }

    AddMenuItem("Import", std::move(menuItem), [this](const std::string& menuItemName)
        {
            // default is do nothing. Owners of MenuItwm objects should call AddCallback to make the MenuItem do something
        });


    AddMenuItem("Exit", std::make_unique<MenuItem>(), [this](const std::string& menuItemName)
        {

        });

}

void GameEngine::FileDropDownMenu::InitializeMenuEntry(MenuItem* menuItem, const std::string& label, int x, int y, float scale, int color)
{
    int menuXPos = 10;
    int menuYPos = 45;
    int menuWidth = 200;

    if (menuItem)
    {
        if (label == "Import")
        {
            menuItem->Initialize("IMPORT", 30, 64, menuXPos, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
        }
        else if (label == "Exit")
        {
            menuItem->Initialize("EXIT", 30, 110, menuXPos, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
        }
    }
    else
    {
        std::cerr << "Error: Null MenuItem pointer for item: " << label << std::endl;
    }
}
