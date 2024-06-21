#include "FileContextMenu.h"

using namespace GameEngine;

FileContextMenu::FileContextMenu() : m_menuBody(std::make_unique<RectangleDrawable>()), m_importBtn(std::make_unique<Button>()), m_exitBtn(std::make_unique<Button>())
{
    m_isActive = false;
	Initialize();
}

void FileContextMenu::Initialize()
{
    m_menuBody->Initialize(10, 45, 200, 100, Colors::BLUE, true);
    m_importBtn->Initialize("IMPORT", 30, 64, 0.65f, Colors::DARK_YELLOW);
    m_exitBtn->Initialize("EXIT", 30, 110, 0.65f, Colors::DARK_YELLOW);
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

void FileContextMenu::Update()
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

void FileContextMenu::AddCallback(Callback callback, FileContextMenu::Choices button)
{
    switch (button)
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

void FileContextMenu::ImportButtonClicked()
{
    m_importBtnCallback(FileContextMenu::Choices::IMPORT);
}

void FileContextMenu::ExitButtonClicked()
{
    m_exitBtnCallback(FileContextMenu::Choices::EXIT);
}

void FileContextMenu::HandleButton(InputManager* InputMgr, Button* button, const std::string& buttonName, std::function<void()> callback)
{
    int mouseX, mouseY;
    InputMgr->GetMousePosition(&mouseX, &mouseY);

    if (InputMgr->m_mouseButtonState[0] && !InputMgr->m_prevMouseButtonState[0] && button->IsMouseOverButton(mouseX, mouseY))
    {
#if DEBUG
        std::cout << buttonName << " button clicked" << std::endl;
#endif
        button->SetButtonColor(Colors::DARK_GRAY);
        callback();
    }
    else if (!InputMgr->m_mouseButtonState[0] && InputMgr->m_prevMouseButtonState[0] && button->IsMouseOverButton(mouseX, mouseY))
    {
#if DEBUG
        std::cout << buttonName << " button released" << std::endl;
#endif
        button->SetButtonColor(Colors::DEFAULT_COLOR);
    }
}

void FileContextMenu::RespondToObserved(InputManager* InputMgr)
{
    HandleButton(InputMgr, m_importBtn.get(), "Import", [this]() { ImportButtonClicked(); });
    HandleButton(InputMgr, m_exitBtn.get(), "Exit", [this]() { ExitButtonClicked(); });
}
