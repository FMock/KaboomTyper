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
    m_inputMessageBox(std::make_shared<InputMessageBox>()),
    m_initialized(false)
{
	Initialize();
}

UIManager::~UIManager()
{
}

void UIManager::Initialize()
{
    // User Input
    m_inputTextBox = std::make_shared<InputTextBox>();
    m_inputTextBox->InitializeTextBox(10, 916, 780, 34, Colors::DEFAULT_COLOR, true);
    m_inputTextBox->AddCallback(std::bind(&UIManager::ProcessInput, this)); // Bind ProcessInput() for use as a Callback by InputTextBox
    m_inputManager->RegisterObserver(m_inputTextBox); // so InputTextbox can respond to user key presses

    // GUI
    m_gamePlayArea->Initialize();
    m_headsUpDisplay->Initialize(445, 43);

    // Register with InputManger to get user updates
    m_inputManager->RegisterObserver(m_gameMenu); // so menu can respond to mouse clicks
    m_inputManager->RegisterObserver(m_fileContextMenu);
    m_inputManager->RegisterObserver(m_inputMessageBox);

    // Register callbacks for main menu
    m_gameMenu->AddCallback([this](Menu::MenuButtons button) { this->DisplayMenuChoices(button); }, Menu::File);
    m_gameMenu->AddCallback([this](Menu::MenuButtons button) { this->DisplayMenuChoices(button); }, Menu::Options);
    m_gameMenu->AddCallback([this](Menu::MenuButtons button) { this->DisplayMenuChoices(button); }, Menu::Help);
    m_gameMenu->AddCallback([this](Menu::MenuButtons button) { this->DisplayMenuChoices(button); }, Menu::About);

    // Register callbacks for sub menus
    m_fileContextMenu->AddCallback([this](FileContextMenu::Choices button) { this->DisplayFileMenuChoices(button); }, FileContextMenu::IMPORT);
    m_fileContextMenu->AddCallback([this](FileContextMenu::Choices button) { this->DisplayFileMenuChoices(button); }, FileContextMenu::EXIT);

    // Input Messagebox to get user name
    // Bind the callback and add the InputTextBox
    //m_inputMessageBox->AddInputTextBox(std::bind(&UIManager::GetUserNamePromptCallback, this));
    m_inputMessageBox->AddInputTextBoxCallback(std::bind(&UIManager::GetUserNamePromptCallback, this));
    m_inputMessageBox->AddButtonCallback(std::bind(&UIManager::GetUserNamePromptCallback, this), InputMessageBox::Buttons::SUBMIT);
    m_inputMessageBox->AddButtonCallback(std::bind(&UIManager::CancelButtonCallback, this), InputMessageBox::Buttons::CANCEL);
    m_inputMessageBox->SetIsActive(true); // ask user for name at start of game
    m_initialized = true;
}

void UIManager::Update(float dt)
{
    m_inputTextBox->Update(dt);
	if (m_headsUpDisplay->UpdateRequired()) // only update the HUD if needed
		m_headsUpDisplay->Update(dt);
}

void UIManager::ProcessInput()
{
    // compare input and active text
    std::string activeStr = Common::GetActiveText();
    std::string submittedStr = Common::GetSubmittedText();

    // Did user score?
    if (activeStr == submittedStr)
    {
        m_processInputCallback(); // Call GameManager's UserScored()
    }

#if DEBUG
    std::cout << "The strings are " << (activeStr == submittedStr ? "equal" : "not equal") << "." << std::endl;
#endif
}

void UIManager::Render()
{
    // DrawOrderManager will manage drawing IDrawable members
}

void UIManager::RegisterDrawables(DrawOrderManager& manager)
{
    // Example priorities
    m_inputTextBox->SetPriority(0);
    m_gamePlayArea->SetPriority(0);
    m_headsUpDisplay->SetPriority(2);
    m_gameMenu->SetPriority(3);
    m_messageBox->SetPriority(4);
    m_fileContextMenu->SetPriority(9);
    m_inputMessageBox->SetPriority(13);

    // Sharing IDrawables with DrawOrderManager
    manager.AddDrawable(m_inputTextBox);
    manager.AddDrawable(m_gamePlayArea);
    manager.AddDrawable(m_headsUpDisplay);
    manager.AddDrawable(m_gameMenu);
    manager.AddDrawable(m_messageBox);
    manager.AddDrawable(m_fileContextMenu);
    manager.AddDrawable(m_inputMessageBox);
}

void GameEngine::UIManager::GetUserNamePromptCallback()
{
    std::cout << "GetUserNamePromptCallback() = " << Common::GetSubmittedText() << std::endl;
    std::string name = Common::GetSubmittedText();
    m_headsUpDisplay->SetUserName(name);
    m_inputMessageBox->SetIsActive(false);
    m_inputTextBox->SetIsActive(true);
}

void GameEngine::UIManager::CancelButtonCallback()
{
    m_inputMessageBox->SetIsActive(false);
    m_inputTextBox->SetIsActive(true);
}

void GameEngine::UIManager::AddCallback(Callback callback)
{
    m_processInputCallback = callback;
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
        m_inputManager->SetShouldQuit(true); // user pressed Exit sub-menu item
#if DEBUG
        std::cout << "Exit The Game" << std::endl;
#endif
        break;
    default:
        break;
    }
}
