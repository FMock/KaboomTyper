#include "HelpDropDownMenu.h"

using namespace GameEngine;

GameEngine::HelpDropDownMenu::HelpDropDownMenu()
{
	InitializeMenuEntries();
	InitializeMenu(X_POSITION, Y_POSITION, WIDTH, HEIGHT, Colors::BLUE, true);
}

void HelpDropDownMenu::ChangeMenuItemLabel(const std::string& key, const std::string& newLabel)
{
    // Check if key exists in the map
    auto it = m_menuItems.find(key);
    if (it == m_menuItems.end())
    {
        throw std::runtime_error("Key '" + key + "' not found in m_menuItems.");
    }

    it->second.menuItem->SetLabelText(newLabel);
}

void HelpDropDownMenu::InitializeMenuEntries()
{
    auto menuItem = std::make_unique<MenuItem>();
    if (!menuItem)
    {
        std::cerr << "Error: Failed to create Menu Item" << std::endl;
    }

    AddMenuItem("Instructions", std::move(menuItem), [this](const std::string& menuItemName)
    {
         // default is do nothing. Owners of MenuItwm objects should call AddCallback to make the MenuItem do something
    });

    AddMenuItem("View Source Code", std::make_unique<MenuItem>(), [this](const std::string& menuItemName)
    {
            // default is do nothing
    });


    AddMenuItem("Send Feedback", std::make_unique<MenuItem>(), [this](const std::string& menuItemName)
    {
            // default is do nothing
    });
}

void HelpDropDownMenu::InitializeMenuEntry(MenuItem* menuItem, const std::string& label, int x, int y, float scale, int color)
{
    int menuXPos = X_POSITION + 10;;
    int menuYPos = Y_POSITION + 5;
    int menuWidth = WIDTH - 10;

    if (menuItem)
    {
        if (label == "Instructions")
        {
            menuItem->Initialize("INSTRUCTIONS", 300, 64, menuXPos, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
        }
        else if (label == "View Source Code")
        {
            menuItem->Initialize("VIEW SOURCE CODE", 300, 110, menuXPos, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
        }
        else if (label == "Send Feedback")
        {
            menuItem->Initialize("SEND FEEDBACK", 300, 156, menuXPos, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
        }
    }
    else
    {
        std::cerr << "Error: Null MenuItem pointer for item: " << label << std::endl;
    }
}
