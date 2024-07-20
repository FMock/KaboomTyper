#include "FileDropDownMenu.h"
#include <stdexcept>
#include <iostream>

GameEngine::FileDropDownMenu::FileDropDownMenu()
{
	InitializeMenuEntries();
	InitializeMenu(10, 40, 200, 110, Colors::BLUE, true);
}


void GameEngine::FileDropDownMenu::ChangeMenuItemLabel(const std::string& key, const std::string& newLabel)
{
    // Check if key exists in the map
    auto it = m_menuItems.find(key);
    if (it == m_menuItems.end())
    {
        throw std::runtime_error("Key '" + key + "' not found in m_menuItems.");
    }

    it->second.menuItem->SetLabelText(newLabel);
}


void GameEngine::FileDropDownMenu::InitializeMenuEntries()
{
    auto menuItem = std::make_unique<MenuItem>();
    if (!menuItem)
    {
        std::cerr << "Error: Failed to create Menu Item" << std::endl;
    }

    AddMenuItem("Start", std::move(menuItem), [this](const std::string& menuItemName)
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
        if (label == "Start")
        {
            menuItem->Initialize("START", 30, 64, menuXPos, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
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
