#include "AudioChoiceMenu.h"

GameEngine::AudioChoiceMenu::AudioChoiceMenu(const std::vector<std::string>& audioOptions) : m_audioOptions(audioOptions)
{
    InitializeMenuEntries();

    const int x = 365;
    const int y = 100;
    int width = 500;
    int height = 860;

    InitializeChoiceMenu(x, y, width, height, Colors::BLUE, true);
}

bool GameEngine::AudioChoiceMenu::GetIsActive(const std::string& keyName)
{
    return m_choiceMenuItems["Play Music"].menuItem->GetIsActive();
}

void GameEngine::AudioChoiceMenu::InitializeMenuEntries()
{
    // Get all audio options
    m_count = m_audioOptions.size();

    for (const auto& category : m_audioOptions)
    {
        auto choiceMenuItemPtr = std::make_unique<ChoiceMenuItem>();
        choiceMenuItemPtr->SetIsSelected(true); // select "Play Audio" is default
        AddChoiceMenuItem(category, std::move(choiceMenuItemPtr), [this](const std::string& menuItemName)
        {
            // default is do nothing. Owners of ChoiceMenuItem objects should call AddCallback to make the choiceMenuItem do something
        });
    }
}

void GameEngine::AudioChoiceMenu::InitializeMenuEntry(ChoiceMenuItem* choiceMenuItem, const std::string& label, int x, int y, float scale, int color)
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
