#include "WordCategoryChoiceMenu.h"
#include "WordManager.h"

using namespace GameEngine;

WordCategoryChoiceMenu::WordCategoryChoiceMenu()
{
    InitializeMenuEntries();

    const int x = 365;
    const int y = 40;
    int width = 500;
    int height = 860;

    InitializeChoiceMenu(x, y, width, height, Colors::BLUE, true);
}


void WordCategoryChoiceMenu::InitializeMenuEntries()
{
    // Get all word categories
    WordManager wordManager;
    auto categories = wordManager.GetWordCategories(); // TODO: GET WORD CATEGORIES FROM GLOBAL SOURCE INSTEAD OF ANOTHER DATABASE QUERY
    m_count = categories.size();



    for (const auto& category : categories)
    {
        auto choiceMenuItemPtr = std::make_unique<ChoiceMenuItem>();
        AddChoiceMenuItem(category, std::move(choiceMenuItemPtr), [this](const std::string& menuItemName)
        {
                // default is do nothing. Owners of ChoiceMenuItem objects should call AddCallback to make the choiceMenuItem do something
        });
    }
}


void WordCategoryChoiceMenu::InitializeMenuEntry(ChoiceMenuItem* choiceMenuItem, const std::string& label, int x, int y, float scale, int color)
{
    if (choiceMenuItem)
    {
        choiceMenuItem->Initialize(label, x, y, scale);
    }
    else
    {
        std::cerr << "Error: Null choiceMenuItem pointer for item: " << label << std::endl;
    }
}
