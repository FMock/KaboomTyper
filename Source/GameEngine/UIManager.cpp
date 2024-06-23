#include "UIManager.h"
#include "Common.h"

#define DEBUG 1

using namespace GameEngine;

UIManager::UIManager(std::shared_ptr<InputManager> inputManager) : 
    m_inputManager(inputManager),
    m_gamePlayArea(std::make_shared<GamePlayArea>()),
    m_headsUpDisplay(std::make_shared<HeadsUpDisplay>()), 
    m_messageBox(std::make_shared<MessageBox>()), 
    m_gameMenu(std::make_shared<Menu>()), 
    m_fileContextMenu(std::make_shared<FileContextMenu>()), 
    m_initialized(false)
{
	Initialize();
}

UIManager::~UIManager()
{
}

void UIManager::Initialize()
{
    m_gamePlayArea->Initialize();
    m_headsUpDisplay->Initialize(445, 43);

    // Register with InputManger to get user updates
    m_inputManager->RegisterObserver(m_gameMenu); // so menu can respond to mouse clicks
    m_inputManager->RegisterObserver(m_fileContextMenu);

    // Register callbacks for main menu
    m_gameMenu->AddCallback([this](Menu::MenuButtons button) { this->DisplayMenuChoices(button); }, Menu::File);
    m_gameMenu->AddCallback([this](Menu::MenuButtons button) { this->DisplayMenuChoices(button); }, Menu::Options);
    m_gameMenu->AddCallback([this](Menu::MenuButtons button) { this->DisplayMenuChoices(button); }, Menu::Help);
    m_gameMenu->AddCallback([this](Menu::MenuButtons button) { this->DisplayMenuChoices(button); }, Menu::About);

    // Register callbacks for sub menus
    m_fileContextMenu->AddCallback([this](FileContextMenu::Choices button) { this->DisplayFileMenuChoices(button); }, FileContextMenu::IMPORT);
    m_fileContextMenu->AddCallback([this](FileContextMenu::Choices button) { this->DisplayFileMenuChoices(button); }, FileContextMenu::EXIT);
    
    m_initialized = true;
}

void UIManager::Update(float dt)
{
	if (m_headsUpDisplay->UpdateRequired()) // only update the HUD if needed
		m_headsUpDisplay->Update(dt);
}

void UIManager::ProcessInput()
{

}

void UIManager::Render()
{
    // DrawOrderManager will manage drawing IDrawable members
}

void UIManager::RegisterDrawables(DrawOrderManager& manager)
{
    // Example priorities
    m_gamePlayArea->SetPriority(0);
    m_headsUpDisplay->SetPriority(2);
    m_gameMenu->SetPriority(3);
    m_messageBox->SetPriority(4);
    m_fileContextMenu->SetPriority(9);

    // Sharing IDrawables with DrawOrderManager
    manager.AddDrawable(m_gamePlayArea);
    manager.AddDrawable(m_headsUpDisplay);
    manager.AddDrawable(m_gameMenu);
    manager.AddDrawable(m_messageBox);
    manager.AddDrawable(m_fileContextMenu);
}

void UIManager::ResetScore()
{
	m_headsUpDisplay->ResetScore();
}

void UIManager::IncreaseScore()
{
	m_headsUpDisplay->IncreaseScore(10); // TODO: HARD CODED AS 10. SCORE INCREASE SHOULD BE BASED ON THE SIZE OF THE TEXT
	m_headsUpDisplay->SetUpdateRequired(true); // HUD needs to be updated
}

void UIManager::GameOver()
{
	m_messageBox->ChangeMessage("GAME OVER", "F1:  NEW GAME", "ESC: EXIT GAME");
}

void GameEngine::UIManager::DisplayMenuChoices(Menu::MenuButtons button)
{
    switch (button)
    {
    case GameEngine::Menu::File:

        m_fileContextMenu->SetIsActive(!m_fileContextMenu->GetIsActive());

#if DEBUG
        std::cout << "Display FileContextMenu" << std::endl;
#endif
        break;
    case GameEngine::Menu::Options:
#if DEBUG
        std::cout << "Display OptionsContextMenu" << std::endl;
#endif
        break;
    case GameEngine::Menu::Help:
#if DEBUG
        std::cout << "Display HelpContextMenu" << std::endl;
#endif
        break;
    case GameEngine::Menu::About:
#if DEBUG
        std::cout << "Display About Menu Choices" << std::endl;
#endif

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
            "About",
            "Kaboom Typer\n A retro inspired typing game by Frank Mock\n https://www.frankmock.com/software/kaboomtyper\n Copyright 2024 All Rights Reserved",
            NULL);

        break;
    default:
        break;
    }
}

void GameEngine::UIManager::DisplayFileMenuChoices(FileContextMenu::Choices button)
{
    switch (button)
    {
    case FileContextMenu::IMPORT:

#if DEBUG
        std::cout << "Display Import Options" << std::endl;
#endif
        break;
    case FileContextMenu::EXIT:
#if DEBUG
        std::cout << "Exit The Game" << std::endl;
#endif
        break;
    default:
        break;
    }
}