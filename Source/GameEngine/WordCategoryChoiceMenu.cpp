#include "WordCategoryChoiceMenu.h"
#include "WordManager.h"

using namespace GameEngine;

WordCategoryChoiceMenu::WordCategoryChoiceMenu(const std::vector<std::string>& wordCategories) : m_wordCategories(wordCategories)
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
    m_count = m_wordCategories.size();

    for (const auto& category : m_wordCategories)
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
