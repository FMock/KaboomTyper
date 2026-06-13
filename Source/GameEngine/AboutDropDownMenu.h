#pragma once

#include "DropDownMenu.h"

namespace GameEngine
{
    // "Home-Brew" style drop-down replacing the old About SDL popup.
    // Shows the game info as menu items; the website item opens in the browser.
    class AboutDropDownMenu : public DropDownMenu
    {
    public:
        AboutDropDownMenu();

    private:
        // Sized/positioned to keep the full URL line within the 800px window.
        const int X_POSITION = 300;
        const int Y_POSITION = 40;
        const int WIDTH = 490;
        const int HEIGHT = 210;
        const float ITEM_SCALE = 0.40f; // smaller than other menus so the full URL fits

        static constexpr const char* WEBSITE_URL = "https://www.frankmock.com/software/kaboomtyper";

    protected:
        void InitializeMenuEntries() override;
        void InitializeMenuEntry(MenuItem* menuItem, const std::string& label, int x, int y, float scale, int color) override;
    };
}
