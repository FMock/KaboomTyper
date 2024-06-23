#include "Menu.h"
#include "GlobalPreprocessorFlags.h"
#include <iostream>

using namespace GameEngine;

Menu::Menu() 
	: m_menuBar(std::make_unique<RectangleDrawable>()), m_optionsBtn(std::make_unique<Button>()), m_aboutBtn(std::make_unique<Button>()), m_helpBtn(std::make_unique<Button>()),
      m_fileBtn(std::make_unique<Button>()), m_footer(std::make_unique<RectangleDrawable>())
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
    m_title.DrawText(1.0f);
    m_footer->Draw();
}

void Menu::Update(float dt)
{
}

void GameEngine::Menu::AddCallback(Callback callback, Menu::MenuButtons button)
{
    switch (button)
    {
    case GameEngine::Menu::File:
        m_fileBtnCallback = callback;
        break;
    case GameEngine::Menu::Options:
        m_optionsBtnCallback = callback;
        break;
    case GameEngine::Menu::Help:
        m_helpBtnCallback = callback;
        break;
    case GameEngine::Menu::About:
        m_aboutBtnCallback = callback;
        break;
    default:
        break;
    }
}

void GameEngine::Menu::Initialize()
{
	m_menuBar->Initialize(0, 0, 900, 42, Colors::BLUE, true);
	m_fileBtn->Initialize("FILE", 15, 12, 0.65f, Colors::DARK_YELLOW);
	m_optionsBtn->Initialize("OPTIONS", 100, 12, 0.65f, Colors::DARK_YELLOW);
	m_helpBtn->Initialize("HELP", 232, 12, 0.65f, Colors::DARK_YELLOW);
	m_aboutBtn->Initialize("ABOUT", 317, 12, 0.65f, Colors::DARK_YELLOW);
    m_title = TextString("Kaboom Typer!", 480, 8);
    m_footer->Initialize(0, 950, 900, 42, Colors::BLUE, true);
}

void GameEngine::Menu::OptionsButtonClicked()
{
    // call the Options button callback
    m_optionsBtnCallback(Menu::MenuButtons::Options);
}

void GameEngine::Menu::FileButtonClicked()
{
    // call the File button callback
    m_fileBtnCallback(Menu::MenuButtons::File);
}

void GameEngine::Menu::HelpButtonClicked()
{
    // call the Help button callback
    m_helpBtnCallback(Menu::MenuButtons::Help);
}

void GameEngine::Menu::AboutButtonClicked()
{
    // call the About button callback
    m_aboutBtnCallback(Menu::MenuButtons::About);
}

void Menu::HandleButton(InputManager* InputMgr, Button* button, const std::string& buttonName, std::function<void()> callback)
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

void Menu::RespondToObserved(InputManager* InputMgr)
{
    HandleButton(InputMgr, m_fileBtn.get(), "File", [this]() { FileButtonClicked(); });
    HandleButton(InputMgr, m_optionsBtn.get(), "Options", [this]() { OptionsButtonClicked(); });
    HandleButton(InputMgr, m_helpBtn.get(), "Help", [this]() { HelpButtonClicked(); });
    HandleButton(InputMgr, m_aboutBtn.get(), "About", [this]() { AboutButtonClicked(); });
}

