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
    // Submenus for each main menu
    m_dropDownMenus[Menu::File] = std::make_shared<FileDropDownMenu>();
    m_dropDownMenus[Menu::Options] = std::make_shared<OptionsDropDownMenu>();

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
    
    // Register each drop-down menu as an observer
    for (const auto& pair : m_dropDownMenus)
    {
        m_inputManager->RegisterObserver(pair.second);
    }
    m_inputManager->RegisterObserver(m_inputMessageBox);

    // Register callbacks for sub menus
    RegisterCallbacks();

    m_initialized = true;
}

void GameEngine::UIManager::RegisterCallbacks()
{
    // Register callbacks for main menu
    m_gameMenu->AddCallback([this](Menu::MenuButtons button) { this->DisplayMenuChoices(button); }, Menu::File);
    m_gameMenu->AddCallback([this](Menu::MenuButtons button) { this->DisplayMenuChoices(button); }, Menu::Options);
    m_gameMenu->AddCallback([this](Menu::MenuButtons button) { this->DisplayMenuChoices(button); }, Menu::Help);
    m_gameMenu->AddCallback([this](Menu::MenuButtons button) { this->DisplayMenuChoices(button); }, Menu::About);

    if (auto fileMenu = std::dynamic_pointer_cast<FileDropDownMenu>(m_dropDownMenus[Menu::File]))
    {
        fileMenu->AddCallback([this](FileDropDownMenu::Choices choice) { this->DisplayFileMenuChoices(choice); }, FileDropDownMenu::IMPORT);
        fileMenu->AddCallback([this](FileDropDownMenu::Choices choice) { this->DisplayFileMenuChoices(choice); }, FileDropDownMenu::EXIT);
    }

    if (auto optionsMenu = std::dynamic_pointer_cast<OptionsDropDownMenu>(m_dropDownMenus[Menu::Options]))
    {
        optionsMenu->AddCallback([this](OptionsDropDownMenu::Choices choice) { this->DisplayOptionsMenuChoices(choice); }, OptionsDropDownMenu::WORD_CATEGORY);
        optionsMenu->AddCallback([this](OptionsDropDownMenu::Choices choice) { this->DisplayOptionsMenuChoices(choice); }, OptionsDropDownMenu::AUDIO);
    }

    // Bind the callback and add the InputTextBox
    m_inputMessageBox->AddInputTextBoxCallback(std::bind(&UIManager::GetUserNamePromptCallback, this));
    m_inputMessageBox->AddButtonCallback(std::bind(&UIManager::GetUserNamePromptCallback, this), InputMessageBox::Buttons::SUBMIT);
    m_inputMessageBox->AddButtonCallback(std::bind(&UIManager::CancelButtonCallback, this), InputMessageBox::Buttons::CANCEL);
    m_inputMessageBox->SetIsActive(true); // ask user for name at start of game
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
    m_inputTextBox->SetPriority(0);
    m_gamePlayArea->SetPriority(0);
    m_headsUpDisplay->SetPriority(2);
    m_gameMenu->SetPriority(3);
    m_messageBox->SetPriority(4);
    std::dynamic_pointer_cast<IDrawable>(m_dropDownMenus[Menu::File])->SetPriority(9);
    std::dynamic_pointer_cast<IDrawable>(m_dropDownMenus[Menu::Options])->SetPriority(9);
    m_inputMessageBox->SetPriority(13);

    // Sharing IDrawables with DrawOrderManager
    manager.AddDrawable(m_inputTextBox);
    manager.AddDrawable(m_gamePlayArea);
    manager.AddDrawable(m_headsUpDisplay);
    manager.AddDrawable(m_gameMenu);
    manager.AddDrawable(m_messageBox);
    manager.AddDrawable(m_inputMessageBox);

    // Add drop-down menus to the DrawOrderManager
    for (const auto& pair : m_dropDownMenus)
    {
        auto drawable = std::dynamic_pointer_cast<IDrawable>(pair.second);
        manager.AddDrawable(drawable);
    }
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

// Ensures each button's OnClick action is mutually exclusive
void UIManager::DisableAllButtonsExceptThisButton(Menu::MenuButtons button)
{
    for (auto& pair : m_dropDownMenus)
    {
        if (pair.first != button)
        {
            if (pair.second->GetIsActive())
            {
                pair.second->SetIsActive(false);
            }
        }
    }
}


void UIManager::DisplayMenuChoices(Menu::MenuButtons button)
{
    switch (button)
    {
    case GameEngine::Menu::File:

        m_dropDownMenus[Menu::File]->SetIsActive(!m_dropDownMenus[Menu::File]->GetIsActive());
        DisableAllButtonsExceptThisButton(Menu::MenuButtons::File);

#if DEBUG
        std::cout << "Display File Drop Down Menu" << std::endl;
#endif
        break;
    case GameEngine::Menu::Options:

        m_dropDownMenus[Menu::Options]->SetIsActive(!m_dropDownMenus[Menu::Options]->GetIsActive());
        DisableAllButtonsExceptThisButton(Menu::MenuButtons::Options);

#if DEBUG
        std::cout << "Display Options Drop Down Menu" << std::endl;
#endif
        break;
    case GameEngine::Menu::Help:
#if DEBUG
        std::cout << "Display Help Drop Down Menu" << std::endl;
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

void GameEngine::UIManager::DisplayFileMenuChoices(FileDropDownMenu::Choices choice)
{
    switch (choice)
    {
    case FileDropDownMenu::IMPORT:

#if DEBUG
        std::cout << "Display Import Options" << std::endl;
#endif
        break;
    case FileDropDownMenu::EXIT:
        m_inputManager->SetShouldQuit(true); // user pressed Exit sub-menu item
#if DEBUG
        std::cout << "Exit The Game" << std::endl;
#endif
        break;
    default:
        break;
    }
}

void GameEngine::UIManager::DisplayOptionsMenuChoices(OptionsDropDownMenu::Choices choice)
{
    switch (choice)
    {
    case OptionsDropDownMenu::Choices::WORD_CATEGORY:

#if DEBUG
        std::cout << "Display Word Category Options" << std::endl;
#endif
        break;
    case OptionsDropDownMenu::Choices::AUDIO:
        
#if DEBUG
        std::cout << "Display Audio Options" << std::endl;
#endif
        break;
    default:
        break;
    }
}
