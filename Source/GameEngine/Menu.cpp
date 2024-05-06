#include "Menu.h"
#include <iostream>

using namespace GameEngine;

Menu::Menu() 
	: m_menuBar(std::make_unique<RectangleDrawable>()), m_optionsBtn(std::make_unique<Button>()), m_aboutBtn(std::make_unique<Button>()), m_helpBtn(std::make_unique<Button>()),
      m_fileBtn(std::make_unique<Button>())
{
	Initialize();
}

Menu::~Menu()
{
}

void Menu::Draw()
{
	m_menuBar->Draw();
	m_fileBtn->Draw();
	m_optionsBtn->Draw();
	m_helpBtn->Draw();
	m_aboutBtn->Draw();
}

void Menu::Update()
{
}

void GameEngine::Menu::Initialize()
{
	m_menuBar->Initialize(0, 0, 900, 34, Colors::YELLOW, true);
	m_fileBtn->Initialize("FILE", 5, 8, 0.65f, Colors::DARK_YELLOW);
	m_optionsBtn->Initialize("OPTIONS", 82, 8, 0.65f, Colors::DARK_YELLOW);
	m_helpBtn->Initialize("HELP", 210, 8, 0.65f, Colors::DARK_YELLOW);
	m_aboutBtn->Initialize("ABOUT", 290, 8, 0.65f, Colors::DARK_YELLOW);

}

void Menu::RespondToObserved(InputManager* InputMgr)
{
    // Access mouse button state information
    if (InputMgr->m_mouseButtonState[0] && !InputMgr->m_prevMouseButtonState[0])
    {
        // Left mouse button clicked
        std::cout << "Left mouse button clicked" << std::endl;
        m_fileBtn->SetButtonColor(Colors::DEFAULT_COLOR);
    }
    else if (!InputMgr->m_mouseButtonState[0] && InputMgr->m_prevMouseButtonState[0])
    {
        // Left mouse button released
        std::cout << "Left mouse button released" << std::endl;
        m_fileBtn->SetButtonColor(Colors::DARK_YELLOW);
    }

    if (InputMgr->m_mouseButtonState[2] && !InputMgr->m_prevMouseButtonState[2])
    {
        // Right mouse button clicked
        std::cout << "Right mouse button clicked" << std::endl;
    }
    else if (!InputMgr->m_mouseButtonState[2] && InputMgr->m_prevMouseButtonState[2])
    {
        // Right mouse button released
        std::cout << "Right mouse button released" << std::endl;
    }

    // Get mouse position
    int mouseX, mouseY;
    InputMgr->GetMousePosition(&mouseX, &mouseY);
    //std::cout << "Mouse position: (" << mouseX << ", " << mouseY << ")" << std::endl;
}

