#include "FileContextMenu.h"

using namespace GameEngine;

FileContextMenu::FileContextMenu() : m_menuBody(std::make_unique<RectangleDrawable>()), m_importBtn(std::make_unique<MenuItem>()), m_exitBtn(std::make_unique<MenuItem>())
{
    m_isActive = false;
	Initialize();
}

void FileContextMenu::Initialize()
{
    int menuXPos = 10;
    int menuYPos = 45;
    int menuWidth = 200;

    m_menuBody->Initialize(menuXPos, menuYPos, menuWidth, 100, Colors::BLUE, true);
    m_importBtn->Initialize("IMPORT", 30, 64, menuXPos, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
    m_exitBtn->Initialize("EXIT", 30, 110, menuXPos, menuYPos, menuWidth, 0.65f, Colors::DARK_YELLOW);
}

void FileContextMenu::Draw()
{
    if (m_isActive)
    {
        m_menuBody->Draw();
        m_importBtn->Draw();
        m_exitBtn->Draw();
    }
}

void FileContextMenu::Update(float dt)
{
}

bool FileContextMenu::GetIsActive() const
{
    return m_isActive;
}

void FileContextMenu::SetIsActive(bool isActive)
{
    m_isActive = isActive;
}

void FileContextMenu::AddCallback(Callback callback, FileContextMenu::Choices MenuItem)
{
    switch (MenuItem)
    {
    case GameEngine::FileContextMenu::IMPORT:
        m_importBtnCallback = callback;
        break;
    case GameEngine::FileContextMenu::EXIT:
        m_exitBtnCallback = callback;
        break;
    default:
        break;
    }
}

void FileContextMenu::ImportMenuItemClicked()
{
    m_importBtnCallback(FileContextMenu::Choices::IMPORT);
}

void FileContextMenu::ExitMenuItemClicked()
{
    m_exitBtnCallback(FileContextMenu::Choices::EXIT);
}

void FileContextMenu::HandleMenuItem(InputManager* InputMgr, MenuItem* MenuItem, const std::string& MenuItemName, std::function<void()> callback)
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

void FileContextMenu::RespondToObserved(InputManager* InputMgr)
{
    HandleMenuItem(InputMgr, m_importBtn.get(), "Import", [this]() { ImportMenuItemClicked(); });
    HandleMenuItem(InputMgr, m_exitBtn.get(), "Exit", [this]() { ExitMenuItemClicked(); });
}
