#include "FileDropDownMenu.h"

using namespace GameEngine;

FileDropDownMenu::FileDropDownMenu() : m_menuBody(std::make_unique<RectangleDrawable>()), 
                                       m_importMenuItem(std::make_unique<MenuItem>()), m_exitMenuItem(std::make_unique<MenuItem>()), 
                                       m_priority(0)
{
    SetIsActive(false);
	Initialize();
}

void FileDropDownMenu::Initialize()
{
    int menuXPos = 10;
    int menuYPos = 45;
    int menuWidth = 200;

    m_menuBody->Initialize(menuXPos, menuYPos, menuWidth, 100, Colors::BLUE, true);
    m_importMenuItem->Initialize("IMPORT", 30, 64, menuXPos, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
    m_exitMenuItem->Initialize("EXIT", 30, 110, menuXPos, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
}

void FileDropDownMenu::Draw()
{
    if (GetIsActive())
    {
        m_menuBody->Draw();
        m_importMenuItem->Draw();
        m_exitMenuItem->Draw();
    }
}

void FileDropDownMenu::Update(float dt)
{
}

void FileDropDownMenu::AddCallback(Callback callback, FileDropDownMenu::Choices MenuItem)
{
    switch (MenuItem)
    {
    case GameEngine::FileDropDownMenu::IMPORT:
        m_importBtnCallback = callback;
        break;
    case GameEngine::FileDropDownMenu::EXIT:
        m_exitBtnCallback = callback;
        break;
    default:
        break;
    }
}

void FileDropDownMenu::ImportMenuItemClicked()
{
    m_importBtnCallback(FileDropDownMenu::Choices::IMPORT);
}

void FileDropDownMenu::ExitMenuItemClicked()
{
    m_exitBtnCallback(FileDropDownMenu::Choices::EXIT);
}

void FileDropDownMenu::HandleMenuItem(InputManager* InputMgr, MenuItem* MenuItem, const std::string& MenuItemName, std::function<void()> callback)
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

void FileDropDownMenu::RespondToObserved(InputManager* InputMgr)
{
    if (GetIsActive())
    {
        HandleMenuItem(InputMgr, m_importMenuItem.get(), "Import", [this]() { ImportMenuItemClicked(); });
        HandleMenuItem(InputMgr, m_exitMenuItem.get(), "Exit", [this]() { ExitMenuItemClicked(); });
    }
}
