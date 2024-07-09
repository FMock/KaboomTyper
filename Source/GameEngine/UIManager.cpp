#include "UIManager.h"
#include "Common.h"
#include "MainMenu.h"
#include <iostream>
#include <stdexcept>

#define DEBUG 1

using namespace GameEngine;

UIManager::UIManager(std::shared_ptr<InputManager> inputManager) :
    m_inputManager(inputManager),
    m_gamePlayArea(std::make_shared<GamePlayArea>()),
    m_headsUpDisplay(std::make_shared<HeadsUpDisplay>()),
    m_messageBox(std::make_shared<MessageBox>()),
    m_gameMenu(std::make_shared<MainMenu>()),
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
    // Get word categories for WordCategoryChoiceMenu
    WordManager wordManager;
    m_wordCategories = wordManager.GetWordCategories();
    if (m_wordCategories.empty())
        std::cerr << "Error: m_wordCategories is empty!" << std::endl;

    // Submenus for each main menu
    m_dropDownMenus["File"] = std::make_shared<FileDropDownMenu>();
    m_dropDownMenus["Options"] = std::make_shared<OptionsDropDownMenu>();

    // CoiceMenus for each drop down menu
    m_choiceMenus["Word Category"] = std::make_shared<WordCategoryChoiceMenu>();

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

    // Register each ChoiceMenu as an observer
    for (const auto& pair : m_choiceMenus)
    {
        m_inputManager->RegisterObserver(pair.second);
    }

    m_inputManager->RegisterObserver(m_inputMessageBox);

    // Register callbacks for submenus
    const char* errorMsg = "UIManager::Initialize(), Call to RegisterCallbacks() returned false";
    if (!RegisterCallbacks())
    {
        std::cerr << errorMsg << std::endl;
        throw std::runtime_error(errorMsg);
    }

    m_initialized = true;
}


bool GameEngine::UIManager::RegisterCallbacks()
{
    // Register callbacks for main menu
    if (!m_gameMenu->AddCallback("File", [this](const std::string& buttonName) { this->DisplayMainMenuChoices(buttonName); }))
    {
        std::cerr << "Failed to register callback for File button" << std::endl;
        return false;
    }

    if (!m_gameMenu->AddCallback("Options", [this](const std::string& buttonName) { this->DisplayMainMenuChoices(buttonName); }))
    {
        std::cerr << "Failed to register callback for Options button" << std::endl;
        return false;
    }

    if (!m_gameMenu->AddCallback("Help", [this](const std::string& buttonName) { this->DisplayMainMenuChoices(buttonName); }))
    {
        std::cerr << "Failed to register callback for Help button" << std::endl;
        return false;
    }

    if (!m_gameMenu->AddCallback("About", [this](const std::string& buttonName) { this->DisplayMainMenuChoices(buttonName); }))
    {
        std::cerr << "Failed to register callback for About button" << std::endl;
        return false;
    }

    // Register callbacks for FileDropDownMenu
    if (auto fileMenu = std::dynamic_pointer_cast<FileDropDownMenu>(m_dropDownMenus["File"]))
    {
        if (!fileMenu->AddCallback("Import", [this](const std::string& choice) { this->DisplayDropDownMenuChoices(choice); }))
        {
            std::cerr << "Failed to register callback for Import MenuItem" << std::endl;
            return false;
        }

        if (!fileMenu->AddCallback("Exit", [this](const std::string& choice) { this->DisplayDropDownMenuChoices(choice); }))
        {
            std::cerr << "Failed to register callback for Exit MenuItem" << std::endl;
            return false;
        }
    }
    else
    {
        std::cerr << "Failed to find FileDropDownMenu" << std::endl;
        return false;
    }

    // Register callbacks for OptionsDropDownMenu
    if (auto optionsMenu = std::dynamic_pointer_cast<OptionsDropDownMenu>(m_dropDownMenus["Options"]))
    {
        if (!optionsMenu->AddCallback("Word Category", [this](const std::string& choice) { this->DisplayDropDownMenuChoices(choice); }))
        {
            std::cerr << "Failed to register callback for Word Category MenuItem" << std::endl;
            return false;
        }

        if (!optionsMenu->AddCallback("Audio", [this](const std::string& choice) { this->DisplayDropDownMenuChoices(choice); }))
        {
            std::cerr << "Failed to register callback for Audio MenuItem" << std::endl;
            return false;
        }
    }
    else
    {
        std::cerr << "Failed to find OptionsDropDownMenu" << std::endl;
        return false;
    }

    //Register callbacks for WordCategoryChoiceMenu
    if (auto choiceMenu = std::dynamic_pointer_cast<WordCategoryChoiceMenu>(m_choiceMenus["Word Category"]))
    {
        for (auto& category : m_wordCategories) 
        {
            if (!choiceMenu->AddCallback(category, [this](const std::string& category) { this->DisplayWordCategoryChoices(category); }))
            {
                std::cerr << "Failed to register callback for " << category << " ChoiceMenuItem" << std::endl;
                return false;
            }
        }
    }
    else
    {
        std::cerr << "Failed to find WordCategoryChoiceMenu" << std::endl;
        return false;
    }

    // Bind the callback and add the InputTextBox
    m_inputMessageBox->AddInputTextBoxCallback(std::bind(&UIManager::GetUserNamePromptCallback, this));
    m_inputMessageBox->AddButtonCallback(std::bind(&UIManager::GetUserNamePromptCallback, this), InputMessageBox::Buttons::SUBMIT);
    m_inputMessageBox->AddButtonCallback(std::bind(&UIManager::CancelButtonCallback, this), InputMessageBox::Buttons::CANCEL);
    m_inputMessageBox->SetIsActive(true); // ask user for name at start of game

    return true;
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
    std::dynamic_pointer_cast<IDrawable>(m_dropDownMenus["File"])->SetPriority(9);
    std::dynamic_pointer_cast<IDrawable>(m_dropDownMenus["Options"])->SetPriority(9);
    std::dynamic_pointer_cast<IDrawable>(m_choiceMenus["Word Category"])->SetPriority(9);
    m_inputMessageBox->SetPriority(13);

    // Sharing IDrawables with DrawOrderManager
    manager.AddDrawable(m_inputTextBox);
    manager.AddDrawable(m_gamePlayArea);
    manager.AddDrawable(m_headsUpDisplay);
    manager.AddDrawable(m_gameMenu);
    manager.AddDrawable(m_messageBox);
    manager.AddDrawable(m_inputMessageBox);

    // Add all menus to the DrawOrderManager
    for (const auto& pair : m_dropDownMenus)
    {
        auto drawable = std::dynamic_pointer_cast<IDrawable>(pair.second);
        manager.AddDrawable(drawable);
    }

    for (const auto& pair : m_choiceMenus)
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

void UIManager::DisableAllButtonsExceptThisButton(const std::string& menuName)
{
    for (auto& pair : m_dropDownMenus)
    {
        if (pair.first != menuName)
        {
            pair.second->SetIsActive(false);
        }
    }

    for (auto& pair : m_choiceMenus)
    {
        if (pair.first != menuName)
        {
            pair.second->SetIsActive(false);
        }
    }
}


void GameEngine::UIManager::DisplayMainMenuChoices(const std::string& buttonName)
{
    auto it = m_dropDownMenus.find(buttonName);
    if (it != m_dropDownMenus.end())
    {
        it->second->SetIsActive(!it->second->GetIsActive());
        DisableAllButtonsExceptThisButton(buttonName);

#if DEBUG
        std::cout << "Display " << buttonName << " Drop Down Menu" << std::endl;
#endif
    }
    else if (buttonName == "Help")
    {
#if DEBUG
        std::cout << "Display Help Drop Down Menu" << std::endl;
#endif
        // Handle Help logic here if needed
    }
    else if (buttonName == "About")
    {
#if DEBUG
        std::cout << "Display About Menu Choices" << std::endl;
#endif

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
            "About",
            "Kaboom Typer\n A retro inspired typing game by Frank Mock \n https://www.frankmock.com/software/kaboomtyper\n Copyright 2024 All Rights Reserved",
            NULL);
    }
    else
    {
#if DEBUG
        std::cout << "Unknown button: " << buttonName << std::endl;
#endif
    }
}

void GameEngine::UIManager::DisplayDropDownMenuChoices(const std::string& choice)
{
    if (choice == "Import")
    {
#if DEBUG
        std::cout << "Display Import Options" << std::endl;
#endif
    }
    else if (choice == "Exit")
    {
        m_inputManager->SetShouldQuit(true); // user pressed Exit sub-menu item
#if DEBUG
        std::cout << "Exit The Game" << std::endl;
#endif
    }
    else if (choice == "Word Category")
    {
#if DEBUG
        std::cout << "Display Word Category" << std::endl;
#endif
        m_choiceMenus["Word Category"]->SetIsActive(true);
    }
    else if (choice == "Audio")
    {
#if DEBUG
        std::cout << "Display Audio Options" << std::endl;
#endif
    }
    else
    {
        // Handle default case if necessary
    }
}


void GameEngine::UIManager::DisplayWordCategoryChoices(const std::string& choice)
{
    for (const auto& categoy : m_wordCategories)
    {
        if (categoy == choice)
        {
            std::cout << "Player has choosen WordCategoryChoice " << categoy << std::endl;
        }
    }
}