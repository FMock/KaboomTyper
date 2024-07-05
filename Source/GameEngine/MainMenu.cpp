#include "MainMenu.h"
#include "Button.h"
#include <iostream>

namespace GameEngine
{
    MainMenu::MainMenu()
    {
        InitializeMenuItems();
        InitializeMenu("Kaboom Typer!", 480, 8, 0, 0, 900, 42, Colors::BLUE, true); // menu text, position of text, menu position, size, and color. 'true' means fill the menu with color 
    }

    void MainMenu::InitializeMenuItems()
    {
        auto fileButton = std::make_unique<Button>();
        if (!fileButton)
        {
            std::cerr << "Error: Failed to create File button" << std::endl;
        }
        AddMenuItem("File", std::move(fileButton), [this](const std::string& buttonName) 
            {
                // default is do nothing. Owners of Menu objects should call AddCallback to make the button do something
            });

        AddMenuItem("Options", std::make_unique<Button>(), [this](const std::string& buttonName)
            {

            });

        AddMenuItem("Help", std::make_unique<Button>(), [this](const std::string& buttonName)
            {
                
            });

        AddMenuItem("About", std::make_unique<Button>(), [this](const std::string& buttonName)
            {

            });
    }

    void MainMenu::InitializeMenuItem(Button* button, const std::string& label, int x, int y, float scale, int color)
    {
        if (button)
        {
            if (label == "File")
            {
                button->Initialize("FILE", 15, 12, 0.65f, Colors::DARK_YELLOW);
            }
            else if (label == "Options")
            {
                button->Initialize("OPTIONS", 100, 12, 0.65f, Colors::DARK_YELLOW);
            }
            else if (label == "Help")
            {
                button->Initialize("HELP", 232, 12, 0.65f, Colors::DARK_YELLOW);
            }
            else if (label == "About")
            {
                button->Initialize("ABOUT", 317, 12, 0.65f, Colors::DARK_YELLOW);
            }
        }
        else
        {
            std::cerr << "Error: Null button pointer for item: " << label << std::endl;
        }
    }
}
