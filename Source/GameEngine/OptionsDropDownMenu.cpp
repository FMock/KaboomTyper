#include "OptionsDropDownMenu.h"

using namespace GameEngine;

OptionsDropDownMenu::OptionsDropDownMenu() : m_menuBody(std::make_unique<RectangleDrawable>()),
                                             m_wordCategoryMenuItem(std::make_unique<MenuItem>()), m_audioMenuItem(std::make_unique<MenuItem>()),
m_priority(0)
{
    m_isActive = false;
    Initialize();
}

void OptionsDropDownMenu::Initialize()
{
    int menuXPos = 125;
    int menuYPos = 45;
    int menuWidth = 240;

    m_menuBody->Initialize(menuXPos, menuYPos, menuWidth, 100, Colors::BLUE, true);
    m_wordCategoryMenuItem->Initialize("WORD CATEGORY", menuXPos + 20, 64, menuXPos + 6, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
    m_audioMenuItem->Initialize("AUDIO", menuXPos + 20, 110, menuXPos + 6, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
}

void OptionsDropDownMenu::Draw()
{
    if (m_isActive)
    {
        m_menuBody->Draw();
        m_wordCategoryMenuItem->Draw();
        m_audioMenuItem->Draw();
    }
}

void OptionsDropDownMenu::Update(float dt)
{
}

bool OptionsDropDownMenu::GetIsActive() const
{
    return m_isActive;
}

void OptionsDropDownMenu::SetIsActive(bool isActive)
{
    m_isActive = isActive;
}

void OptionsDropDownMenu::AddCallback(Callback callback, OptionsDropDownMenu::Choices MenuItem)
{
    switch (MenuItem)
    {
    case GameEngine::OptionsDropDownMenu::WORD_CATEGORY:
        m_wordCategoryMenuItemCallback = callback;
        break;
    case GameEngine::OptionsDropDownMenu::AUDIO:
        m_audioMenuItemCallback = callback;
        break;
    default:
        break;
    }
}

void GameEngine::OptionsDropDownMenu::WordCategoryMenuItemClicked()
{
    m_wordCategoryMenuItemCallback(OptionsDropDownMenu::Choices::WORD_CATEGORY);
}

void GameEngine::OptionsDropDownMenu::AudioMenuItemClicked()
{
    m_audioMenuItemCallback(OptionsDropDownMenu::Choices::AUDIO);
}

void OptionsDropDownMenu::HandleMenuItem(InputManager* InputMgr, MenuItem* MenuItem, const std::string& MenuItemName, std::function<void()> callback)
{
    int mouseX, mouseY;

    InputMgr->GetMousePosition(&mouseX, &mouseY);

    MenuItem->SetIsActive(MenuItem->IsMouseOverMenuItem(mouseX, mouseY)); // toggle current MenuItem active state

    if (InputMgr->m_mouseButtonState[0] && !InputMgr->m_prevMouseButtonState[0] && MenuItem->IsMouseOverMenuItem(mouseX, mouseY))
    {
#if DEBUG
        std::cout << MenuItemName << " MenuItem clicked" << std::endl;
#endif
        MenuItem->SetMenuItemColor(Colors::DARK_GRAY);
        callback();
    }
    else if (!InputMgr->m_mouseButtonState[0] && InputMgr->m_prevMouseButtonState[0] && MenuItem->IsMouseOverMenuItem(mouseX, mouseY))
    {
#if DEBUG
        std::cout << MenuItemName << " MenuItem released" << std::endl;
#endif
        MenuItem->SetMenuItemColor(Colors::DEFAULT_COLOR);
    }
}

void OptionsDropDownMenu::RespondToObserved(InputManager* InputMgr)
{
    if (m_isActive)
    {
        HandleMenuItem(InputMgr, m_wordCategoryMenuItem.get(), "Word Category", [this]() { WordCategoryMenuItemClicked(); });
        HandleMenuItem(InputMgr, m_audioMenuItem.get(), "Audio", [this]() { AudioMenuItemClicked(); });
    }
}