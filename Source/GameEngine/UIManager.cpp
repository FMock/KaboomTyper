#include "UIManager.h"
#include "Common.h"
#include "MainMenu.h"
#include "AudioChoiceMenu.h"
#include "WordSpeedChoiceMenu.h"
#include <iostream>
#include <stdexcept>

#define DEBUG 1

using namespace GameEngine;

UIManager::UIManager(std::shared_ptr<InputManager> inputManager, std::shared_ptr<WordManager> wordManager) :
    m_inputManager(inputManager),
    m_gamePlayArea(std::make_shared<GamePlayArea>()),
    m_headsUpDisplay(std::make_shared<HeadsUpDisplay>()),
    m_messageBox(std::make_shared<MessageBox>()),
    m_gameMenu(std::make_shared<MainMenu>()),
    m_inputMessageBox(std::make_shared<InputMessageBox>()),
    m_wordManager(wordManager),
    m_wordSpeedOptions{ "Default", "Hare", "Turtle", "Cheetah" },
    m_audioOptions{"Play Music"},
    m_initialized(false)
{
	Initialize();
}

void UIManager::Initialize()
{
    // Get word categories for WordCategoryChoiceMenu
    m_wordCategories = m_wordManager->GetWordCategories();
    if (m_wordCategories.empty())
        std::cerr << "Error: m_wordCategories is empty!" << std::endl;

    // Submenus for each main menu
    m_dropDownMenus["File"] = std::make_shared<FileDropDownMenu>();
    m_dropDownMenus["Options"] = std::make_shared<OptionsDropDownMenu>();
    m_dropDownMenus["Help"] = std::make_shared<HelpDropDownMenu>();
    m_dropDownMenus["About"] = std::make_shared<AboutDropDownMenu>();

    // CoiceMenus for each drop down menu
    m_choiceMenus["Word Category"] = std::make_shared<WordCategoryChoiceMenu>(m_wordCategories);
    m_choiceMenus["Word Speed"] = std::make_shared<WordSpeedChoiceMenu>(m_wordSpeedOptions);
    m_choiceMenus["Audio"] = std::make_shared<AudioChoiceMenu>(m_audioOptions);

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
        if (!fileMenu->AddCallback("Start", [this](const std::string& choice) { this->FileDropDownMenuOnClick(choice); }))
        {
            std::cerr << "Failed to register callback for Import MenuItem" << std::endl;
            return false;
        }

        if (!fileMenu->AddCallback("Exit", [this](const std::string& choice) { this->FileDropDownMenuOnClick(choice); }))
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
        if (!optionsMenu->AddCallback("Word Category", [this](const std::string& choice) { this->OptionsDropDownMenuOnClick(choice); }))
        {
            std::cerr << "Failed to register callback for Word Category MenuItem" << std::endl;
            return false;
        }

        if (!optionsMenu->AddCallback("Audio", [this](const std::string& choice) { this->OptionsDropDownMenuOnClick(choice); }))
        {
            std::cerr << "Failed to register callback for Audio MenuItem" << std::endl;
            return false;
        }

        if (!optionsMenu->AddCallback("Word Speed", [this](const std::string& choice) { this->OptionsDropDownMenuOnClick(choice); }))
        {
            std::cerr << "Failed to register callback for Word Speed MenuItem" << std::endl;
            return false;
        }
    }
    else
    {
        std::cerr << "Failed to find OptionsDropDownMenu" << std::endl;
        return false;
    }

    // Register callbacks for OptionsDropDownMenu
    if (auto helpMenu = std::dynamic_pointer_cast<HelpDropDownMenu>(m_dropDownMenus["Help"]))
    {
        if (!helpMenu->AddCallback("Instructions", [this](const std::string& choice) { this->HelpDropDownMenuOnClick(choice); }))
        {
            std::cerr << "Failed to register callback for Instructions MenuItem" << std::endl;
            return false;
        }

        if (!helpMenu->AddCallback("View Source Code", [this](const std::string& choice) { this->HelpDropDownMenuOnClick(choice); }))
        {
            std::cerr << "Failed to register callback for View Source Code MenuItem" << std::endl;
            return false;
        }

        if (!helpMenu->AddCallback("Send Feedback", [this](const std::string& choice) { this->HelpDropDownMenuOnClick(choice); }))
        {
            std::cerr << "Failed to register callback for Send Feedback MenuItem" << std::endl;
            return false;
        }
    }
    else
    {
        std::cerr << "Failed to find HelpDropDownMenu" << std::endl;
        return false;
    }

    //Register callbacks for WordCategoryChoiceMenu
    if (auto choiceMenu = std::dynamic_pointer_cast<WordCategoryChoiceMenu>(m_choiceMenus["Word Category"]))
    {
        for (auto& category : m_wordCategories) 
        {
            if (!choiceMenu->AddCallback(category, [this](const std::string& category) { this->WordCategoryChoiceMenuOnClick(category); }))
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

    //Register callbacks for AudioChoiceMenu
    if (auto choiceMenu = std::dynamic_pointer_cast<AudioChoiceMenu>(m_choiceMenus["Audio"]))
    {
        for (auto& option : m_audioOptions)
        {
            if (!choiceMenu->AddCallback(option, [this](const std::string& option) { this->AudioChoiceMenuOnClick(option); }))
            {
                std::cerr << "Failed to register callback for " << option << " ChoiceMenuItem" << std::endl;
                return false;
            }
        }
    }
    else
    {
        std::cerr << "Failed to find AudioChoiceMenu" << std::endl;
        return false;
    }

    //Register callbacks for WordSpeedChoiceMenu
    if (auto choiceMenu = std::dynamic_pointer_cast<WordSpeedChoiceMenu>(m_choiceMenus["Word Speed"]))
    {
        for (auto& option : m_wordSpeedOptions)
        {
            if (!choiceMenu->AddCallback(option, [this](const std::string& option) { this->WordSpeedChoiceMenuOnClick(option); }))
            {
                std::cerr << "Failed to register callback for " << option << " ChoiceMenuItem" << std::endl;
                return false;
            }
        }
    }
    else
    {
        std::cerr << "Failed to find WordSpeedChoiceMenu" << std::endl;
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

    // Call Update() on each DropDownMenu
    for (auto& pair : m_dropDownMenus)
    {
        if (pair.second)
        {
            pair.second->Update(dt);
        }
    }
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
    std::dynamic_pointer_cast<IDrawable>(m_dropDownMenus["Help"])->SetPriority(9);
    std::dynamic_pointer_cast<IDrawable>(m_dropDownMenus["About"])->SetPriority(9);
    std::dynamic_pointer_cast<IDrawable>(m_choiceMenus["Word Category"])->SetPriority(9);
    std::dynamic_pointer_cast<IDrawable>(m_choiceMenus["Word Speed"])->SetPriority(9);
    std::dynamic_pointer_cast<IDrawable>(m_choiceMenus["Audio"])->SetPriority(9);
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

void GameEngine::UIManager::ChangeStartMenuItemLabel(const std::string& newLabelText)
{
    auto menu = std::dynamic_pointer_cast<FileDropDownMenu>(m_dropDownMenus["File"]);
    menu->ChangeMenuItemLabel("Start", newLabelText); // For the menu item with key == "Start", change it's label to newLabelText
}

void GameEngine::UIManager::AddCallback(Callback callback)
{
    m_processInputCallback = callback;
}

void UIManager::AddGameOverCallback(Callback callback)
{
    m_gameOverCallback = callback;
}

void UIManager::AddStartGameCallback(Callback callback)
{
    m_startGameCallback = callback;
}

void UIManager::AddAudioCallback(AudioCallback callback)
{
    m_audioCallback = callback;
}

void UIManager::AddWordSpeedCallback(WordSpeedCallback callback)
{
    m_wordSpeedCallback = callback;
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
    else
    {
#if DEBUG
        std::cout << "Unknown button: " << buttonName << std::endl;
#endif
    }
}

namespace
{
    // True only on the frame the key transitions from up to down (edge detection).
    bool KeyJustPressed(GameEngine::InputManager* in, SDL_Scancode sc)
    {
        return in->m_kbState[sc] && !in->m_kbPrevState[sc];
    }

    bool CtrlHeld(GameEngine::InputManager* in)
    {
        return in->m_kbState[SDL_SCANCODE_LCTRL] || in->m_kbState[SDL_SCANCODE_RCTRL];
    }
}

bool GameEngine::UIManager::IsAnyMenuOpen() const
{
    for (const auto& pair : m_dropDownMenus)
        if (pair.second->GetIsActive()) return true;
    for (const auto& pair : m_choiceMenus)
        if (pair.second->GetIsActive()) return true;
    return false;
}

void GameEngine::UIManager::CloseAllMenus()
{
    for (auto& pair : m_dropDownMenus)
    {
        pair.second->SetIsActive(false);
        pair.second->ResetHighlight();
    }
    for (auto& pair : m_choiceMenus)
    {
        pair.second->SetIsActive(false);
        pair.second->ResetHighlight();
    }
}

std::shared_ptr<GameEngine::DropDownMenu> GameEngine::UIManager::GetOpenDropDown() const
{
    for (const auto& pair : m_dropDownMenus)
        if (pair.second->GetIsActive()) return pair.second;
    return nullptr;
}

std::shared_ptr<GameEngine::ChoiceMenu> GameEngine::UIManager::GetOpenChoiceMenu() const
{
    for (const auto& pair : m_choiceMenus)
        if (pair.second->GetIsActive()) return pair.second;
    return nullptr;
}

void GameEngine::UIManager::OpenMenu(const std::string& name)
{
    auto it = m_dropDownMenus.find(name);
    if (it == m_dropDownMenus.end())
        return;

    it->second->SetIsActive(true);
    DisableAllButtonsExceptThisButton(name);
    it->second->SetHighlight(0); // highlight the first item for keyboard users
}

bool GameEngine::UIManager::HandleMenuInput(InputManager* InputMgr)
{
    // 1. CTRL shortcuts: open the corresponding top menu (toggles drop-downs).
    if (CtrlHeld(InputMgr))
    {
        const char* menuName = nullptr;
        if (KeyJustPressed(InputMgr, SDL_SCANCODE_F)) menuName = "File";
        else if (KeyJustPressed(InputMgr, SDL_SCANCODE_O)) menuName = "Options";
        else if (KeyJustPressed(InputMgr, SDL_SCANCODE_H)) menuName = "Help";
        else if (KeyJustPressed(InputMgr, SDL_SCANCODE_A)) menuName = "About";

        if (menuName)
        {
            DisplayMainMenuChoices(menuName);   // toggles the drop-down (or shows About box)
            if (auto opened = GetOpenDropDown())
                opened->SetHighlight(0);
            return true;
        }
    }

    // 2. Esc: back out one level — close an open choice sub-menu first (focus returns to
    //    the drop-down), then the drop-down; if nothing is open, defer to the caller (quit).
    if (KeyJustPressed(InputMgr, SDL_SCANCODE_ESCAPE))
    {
        if (auto choice = GetOpenChoiceMenu())
        {
            choice->SetIsActive(false);
            choice->ResetHighlight();
            return true;
        }
        if (IsAnyMenuOpen())
        {
            CloseAllMenus();
            return true;
        }
        return false;
    }

    // 3a. Arrow / Enter navigation while a choice sub-menu is open — it has focus.
    if (auto choice = GetOpenChoiceMenu())
    {
        if (KeyJustPressed(InputMgr, SDL_SCANCODE_DOWN))
        {
            choice->MoveHighlight(1);
            return true;
        }
        if (KeyJustPressed(InputMgr, SDL_SCANCODE_UP))
        {
            choice->MoveHighlight(-1);
            return true;
        }
        if (KeyJustPressed(InputMgr, SDL_SCANCODE_RETURN) || KeyJustPressed(InputMgr, SDL_SCANCODE_KP_ENTER))
        {
            choice->ActivateHighlighted(); // select the category/speed/audio option
            return true;
        }
    }
    // 3b. Otherwise, arrow / Enter navigation while a drop-down is open.
    else if (auto open = GetOpenDropDown())
    {
        if (KeyJustPressed(InputMgr, SDL_SCANCODE_DOWN))
        {
            open->MoveHighlight(1);
            return true;
        }
        if (KeyJustPressed(InputMgr, SDL_SCANCODE_UP))
        {
            open->MoveHighlight(-1);
            return true;
        }
        if (KeyJustPressed(InputMgr, SDL_SCANCODE_RIGHT) || KeyJustPressed(InputMgr, SDL_SCANCODE_LEFT))
        {
            // Switch to the adjacent top menu (File/Options/Help), keeping it open.
            const int count = static_cast<int>(m_topMenuOrder.size());
            int curIdx = -1;
            for (int i = 0; i < count; ++i)
            {
                auto it = m_dropDownMenus.find(m_topMenuOrder[i]);
                if (it != m_dropDownMenus.end() && it->second->GetIsActive())
                {
                    curIdx = i;
                    break;
                }
            }
            if (curIdx >= 0 && count > 0)
            {
                int delta = KeyJustPressed(InputMgr, SDL_SCANCODE_RIGHT) ? 1 : -1;
                int nextIdx = ((curIdx + delta) % count + count) % count;
                OpenMenu(m_topMenuOrder[nextIdx]);
            }
            return true;
        }
        if (KeyJustPressed(InputMgr, SDL_SCANCODE_RETURN) || KeyJustPressed(InputMgr, SDL_SCANCODE_KP_ENTER))
        {
            // Activate the highlighted item; an Options item opens its choice sub-menu,
            // which we then give keyboard focus by highlighting its first entry.
            open->ActivateHighlighted();
            if (auto opened = GetOpenChoiceMenu())
                opened->SetHighlight(0);
            return true;
        }
    }

    // 4. Click outside any open menu closes all menus.
    if (InputMgr->m_mouseButtonState[0] && !InputMgr->m_prevMouseButtonState[0] && IsAnyMenuOpen())
    {
        int mouseX, mouseY;
        InputMgr->GetMousePosition(&mouseX, &mouseY);

        bool insideMenus = m_gameMenu->IsPointInMenuBar(mouseX, mouseY); // top bar handles its own toggles
        for (auto& pair : m_dropDownMenus)
            if (pair.second->GetIsActive() && pair.second->IsPointInBody(mouseX, mouseY))
                insideMenus = true;
        for (auto& pair : m_choiceMenus)
            if (pair.second->GetIsActive() && pair.second->IsPointInBody(mouseX, mouseY))
                insideMenus = true;

        if (!insideMenus)
            CloseAllMenus();
        // Do not consume: a plain click should still reach other observers.
    }

    return false;
}

void GameEngine::UIManager::FileDropDownMenuOnClick(const std::string& choice)
{
    if (choice == "START")
    {
#if DEBUG
        std::cout << "Start the game" << std::endl;
#endif
        m_startGameCallback();
    }
    else if (choice == "STOP")
    {
#if DEBUG
        std::cout << "Stop The Game" << std::endl;
#endif
        m_gameOverCallback();
    }
    else if (choice == "EXIT")
    {
        m_inputManager->SetShouldQuit(true); // user pressed Exit sub-menu item
#if DEBUG
        std::cout << "Exit The Game" << std::endl;
#endif
    }
    else
    {
        // Do  Nothing
    }
}

void GameEngine::UIManager::OptionsDropDownMenuOnClick(const std::string& choice)
{
    if (choice == "WORD CATEGORY")
    {
#if DEBUG
        std::cout << "Display Word Category" << std::endl;
#endif
        m_choiceMenus["Word Category"]->SetIsActive(!m_choiceMenus["Word Category"]->GetIsActive());
        if (m_choiceMenus["Word Category"]->GetIsActive())
        {
            m_choiceMenus["Audio"]->SetIsActive(false);
            m_choiceMenus["Word Speed"]->SetIsActive(false);
        }
    }
    else if (choice == "AUDIO")
    {
#if DEBUG
        std::cout << "Display Audio Options" << std::endl;
#endif
        m_choiceMenus["Audio"]->SetIsActive(!m_choiceMenus["Audio"]->GetIsActive());
        if (m_choiceMenus["Audio"]->GetIsActive()) 
        {
            m_choiceMenus["Word Category"]->SetIsActive(false);
            m_choiceMenus["Word Speed"]->SetIsActive(false);
        }
    }
    else if (choice == "WORD SPEED")
    {
#if DEBUG
        std::cout << "Display Word Speed Options" << std::endl;
#endif
        m_choiceMenus["Word Speed"]->SetIsActive(!m_choiceMenus["Word Speed"]->GetIsActive());
        if (m_choiceMenus["Word Speed"]->GetIsActive()) 
        {
            m_choiceMenus["Word Category"]->SetIsActive(false);
            m_choiceMenus["Audio"]->SetIsActive(false);
        }  
    }
    else
    {
        // Do Nothing
    }
}


void GameEngine::UIManager::WordCategoryChoiceMenuOnClick(const std::string& choice)
{
    std::string selection = choice;
    for (const auto& category : m_wordCategories)
    {
        if (category == choice) // valid category
        {
            if (choice == "Default")
                selection = Common::DEFAULT_WORD_CATEGORY;
                
            auto wordCategory = m_wordManager->GetWordCategory(selection);
            m_wordManager->ChangeWordCategory(wordCategory);
            std::cout << "Player has choosen WordCategoryChoice " << selection << std::endl;
        }
    }
}

void GameEngine::UIManager::AudioChoiceMenuOnClick(const std::string& choice)
{
    bool state = false;
    const char* errorMsg = "UIManager::AudioChoiceMenuOnClick, invalid choice key";
    std::string selection = choice;
    for (const auto& option : m_audioOptions)
    {
        if (option == choice) // valid audio option
        {
            std::cout << "Player has choosen audio option " << selection << std::endl;

            auto menu = std::dynamic_pointer_cast<AudioChoiceMenu>(m_choiceMenus["Audio"]);
            state = menu->GetMenuItemSelectionState("Play Music");
        }
        else 
        {
            std::cerr << errorMsg << std::endl;
            throw std::runtime_error(errorMsg);
        }
    }

    std::cout << selection << " state is " << state << std::endl;

    // turns music on or off depending on state of "Play Music" selection
    m_audioCallback(state);
}

void GameEngine::UIManager::WordSpeedChoiceMenuOnClick(const std::string& choice)
{
    bool state = false;
    const char* errorMsg = "UIManager::WordSpeedChoiceMenuOnClick, invalid choice key";
    std::string selection = choice;

    for (const auto& option : m_wordSpeedOptions)
    {
        if (option == choice) // valid word speed option
        {
            auto menu = std::dynamic_pointer_cast<WordSpeedChoiceMenu>(m_choiceMenus["Word Speed"]);
            state = menu->GetMenuItemSelectionState(option);
        }
    }

#if DEBUG
    std::cout << selection << " state is " << state << std::endl;
#endif

    // If state is false then player toggled the current speed off, so set speed selection to Default
    if (!state)
    {
        selection = "Default";
    }

    m_wordSpeedCallback(selection);
}

void GameEngine::UIManager::HelpDropDownMenuOnClick(const std::string& choice)
{
    if (choice == "INSTRUCTIONS")
    {
#if DEBUG
        std::cout << "Display Instructions" << std::endl;
#endif

    }
    else if (choice == "VIEW SOURCE CODE")
    {
#if DEBUG
        std::cout << "Display View Source Code Option" << std::endl;
#endif

    }
    else if (choice == "SEND FEEDBACK")
    {
#if DEBUG
        std::cout << "Display Send Feedback Option" << std::endl;
#endif

    }
    else
    {
        // Do Nothing
    }
}