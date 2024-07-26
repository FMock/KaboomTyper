#include "WordSpeedChoiceMenu.h"

GameEngine::WordSpeedChoiceMenu::WordSpeedChoiceMenu(const std::vector<std::string>& speedOptions) : m_speedOptions(speedOptions)
{
    InitializeMenuEntries();

    m_x = 365;
    m_y = 100;
    m_width = 500;
    m_height = 860;

    InitializeChoiceMenu(m_x, m_y, m_width, m_height, Colors::BLUE, true);
}

bool GameEngine::WordSpeedChoiceMenu::GetIsActive(const std::string& keyName)
{
    return m_choiceMenuItems[keyName].menuItem->GetIsActive();
}

void GameEngine::WordSpeedChoiceMenu::InitializeMenuEntries()
{
    // Get all speed options
    m_count = m_speedOptions.size();

    for (int i = 0; i < m_count; i++)
    {
        auto choiceMenuItemPtr = std::make_unique<ChoiceMenuItem>();
        if (i == 0) 
        {
            choiceMenuItemPtr->SetIsSelected(true); // select first speed (default)
        }

        AddChoiceMenuItem(m_speedOptions[i], std::move(choiceMenuItemPtr), [this](const std::string& menuItemName)
        {
                // default is do nothing. Owners of ChoiceMenuItem objects should call AddCallback to make the choiceMenuItem do something
        });
    }
}

void GameEngine::WordSpeedChoiceMenu::InitializeMenuEntry(ChoiceMenuItem* choiceMenuItem, const std::string& label, int x, int y, float scale, int color)
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
