#include "AboutDropDownMenu.h"
#include <SDL.h>
#include <iostream>

using namespace GameEngine;

GameEngine::AboutDropDownMenu::AboutDropDownMenu()
{
    InitializeMenuEntries();
    InitializeMenu(X_POSITION, Y_POSITION, WIDTH, HEIGHT, Colors::BLUE, true);
}

void AboutDropDownMenu::InitializeMenuEntries()
{
    // Informational items (do nothing when activated), matching the Help menu's style.
    AddMenuItem("Title", std::make_unique<MenuItem>(), [this](const std::string&) {});
    AddMenuItem("Tagline", std::make_unique<MenuItem>(), [this](const std::string&) {});

    // The website item opens the URL in the default browser. The same callback is invoked
    // by both the mouse path (DropDownMenu::HandleMenuItem) and the keyboard path
    // (DropDownMenu::ActivateHighlighted), so click and Enter both work.
    AddMenuItem("Website", std::make_unique<MenuItem>(), [this](const std::string&)
    {
        SDL_OpenURL(WEBSITE_URL);
    });

    AddMenuItem("Copyright", std::make_unique<MenuItem>(), [this](const std::string&) {});
}

void AboutDropDownMenu::InitializeMenuEntry(MenuItem* menuItem, const std::string& label, int x, int y, float scale, int color)
{
    if (!menuItem)
    {
        std::cerr << "Error: Null MenuItem pointer for item: " << label << std::endl;
        return;
    }

    const int menuXPos = X_POSITION + 10;
    const int menuYPos = Y_POSITION + 5;
    const int menuWidth = WIDTH - 10;
    const int textX = X_POSITION + 15;

    // Each item's outline Y is derived from its text Y (see MenuItem::Initialize),
    // so distinct Y values stack the rows top-to-bottom.
    if (label == "Title")
    {
        menuItem->Initialize("KABOOM TYPER", textX, 64, menuXPos, menuYPos, menuWidth, ITEM_SCALE, Colors::DARK_YELLOW);
    }
    else if (label == "Tagline")
    {
        menuItem->Initialize("A RETRO INSPIRED TYPING GAME BY FRANK MOCK", textX, 110, menuXPos, menuYPos, menuWidth, ITEM_SCALE, Colors::DARK_YELLOW);
    }
    else if (label == "Website")
    {
        menuItem->Initialize("HTTPS://WWW.FRANKMOCK.COM/SOFTWARE/KABOOMTYPER", textX, 156, menuXPos, menuYPos, menuWidth, ITEM_SCALE, Colors::DARK_YELLOW);
    }
    else if (label == "Copyright")
    {
        menuItem->Initialize("COPYRIGHT 2024 ALL RIGHTS RESERVED", textX, 202, menuXPos, menuYPos, menuWidth, ITEM_SCALE, Colors::DARK_YELLOW);
    }
}
