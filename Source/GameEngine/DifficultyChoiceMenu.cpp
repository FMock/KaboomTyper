#include "DifficultyChoiceMenu.h"

GameEngine::DifficultyChoiceMenu::DifficultyChoiceMenu(const std::vector<std::string>& difficultyOptions) : m_difficultyOptions(difficultyOptions)
{
    InitializeMenuEntries();

    m_x = 365;
    m_y = 100;
    m_width = 500;
    m_height = 860;

    InitializeChoiceMenu(m_x, m_y, m_width, m_height, Colors::BLUE, true);
}

bool GameEngine::DifficultyChoiceMenu::GetIsActive(const std::string& keyName)
{
    return m_choiceMenuItems[keyName].menuItem->GetIsActive();
}

void GameEngine::DifficultyChoiceMenu::InitializeMenuEntries()
{
    // Get all difficulty options
    m_count = m_difficultyOptions.size();

    for (int i = 0; i < m_count; i++)
    {
        auto choiceMenuItemPtr = std::make_unique<ChoiceMenuItem>();
        if (m_difficultyOptions[i] == "Normal")
        {
            choiceMenuItemPtr->SetIsSelected(true); // Normal is the default difficulty
        }

        AddChoiceMenuItem(m_difficultyOptions[i], std::move(choiceMenuItemPtr), [this](const std::string& menuItemName)
        {
                // default is do nothing. Owners of ChoiceMenuItem objects should call AddCallback to make the choiceMenuItem do something
        });
    }
}

void GameEngine::DifficultyChoiceMenu::InitializeMenuEntry(ChoiceMenuItem* choiceMenuItem, const std::string& label, int x, int y, float scale, int color)
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
