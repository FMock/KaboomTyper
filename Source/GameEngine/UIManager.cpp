#include "UIManager.h"
#include "Common.h"
#include "MainMenu.h"
#include "AudioChoiceMenu.h"
#include "WordSpeedChoiceMenu.h"
#include "DifficultyChoiceMenu.h"
#include <SDL.h>
#include <iostream>
#include <stdexcept>
#include <set>
#include <algorithm>
#include <cctype>

#define DEBUG 1

using namespace GameEngine;

UIManager::UIManager(std::shared_ptr<InputManager> inputManager, std::shared_ptr<WordManager> wordManager) :
    m_inputManager(inputManager),
    m_gamePlayArea(std::make_shared<GamePlayArea>()),
    m_headsUpDisplay(std::make_shared<HeadsUpDisplay>()),
    m_messageBox(std::make_shared<MessageBox>()),
    m_gameMenu(std::make_shared<MainMenu>()),
    m_inputMessageBox(std::make_shared<InputMessageBox>()),
    m_exitConfirmPopup(std::make_shared<PopupMessageBox>()),
    m_instructionsPopup(std::make_shared<PopupMessageBox>()),
    m_feedbackPopup(std::make_shared<PopupMessageBox>()),
    m_sourcePopup(std::make_shared<PopupMessageBox>()),
    m_wordManager(wordManager),
    m_wordSpeedOptions{ "Default", "Hare", "Turtle", "Cheetah" },
    m_audioOptions{"Play Music"},
    m_difficultyOptions{ "Easy", "Normal", "Hard" },
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

    // Group the animal categories under a single "Animals" entry. Everything that is not a
    // known non-animal category is treated as an animal (so new animal categories added to the
    // DB automatically join the group).
    static const std::set<std::string> NON_ANIMALS = { "Eye", "Anime", "Default" };
    m_animalCategories.clear();
    m_topLevelCategories.clear();
    m_topLevelCategories.push_back("Animals");
    for (const auto& category : m_wordCategories)
    {
        if (NON_ANIMALS.count(category))
            m_topLevelCategories.push_back(category); // Eye / Anime / Default stay top-level
        else
            m_animalCategories.push_back(category);   // grouped under the Animals fly-out
    }

    // Submenus for each main menu
    m_dropDownMenus["File"] = std::make_shared<FileDropDownMenu>();
    m_dropDownMenus["Options"] = std::make_shared<OptionsDropDownMenu>();
    m_dropDownMenus["Help"] = std::make_shared<HelpDropDownMenu>();
    m_dropDownMenus["About"] = std::make_shared<AboutDropDownMenu>();

    // CoiceMenus for each drop down menu
    m_choiceMenus["Word Category"] = std::make_shared<WordCategoryChoiceMenu>(m_topLevelCategories);
    m_choiceMenus["Animals"] = std::make_shared<WordCategoryChoiceMenu>(m_animalCategories); // fly-out
    m_choiceMenus["Word Speed"] = std::make_shared<WordSpeedChoiceMenu>(m_wordSpeedOptions);
    m_choiceMenus["Audio"] = std::make_shared<AudioChoiceMenu>(m_audioOptions);
    m_choiceMenus["Difficulty"] = std::make_shared<DifficultyChoiceMenu>(m_difficultyOptions);

    // User Input
    m_inputTextBox = std::make_shared<InputTextBox>();
    m_inputTextBox->InitializeTextBox(10, 916, 880, 34, Colors::DEFAULT_COLOR, true);
    m_inputTextBox->AddCallback(std::bind(&UIManager::ProcessInput, this)); // Bind ProcessInput() for use as a Callback by InputTextBox
    m_inputManager->RegisterObserver(m_inputTextBox); // so InputTextbox can respond to user key presses

    // GUI
    m_gamePlayArea->Initialize();
    m_headsUpDisplay->Initialize(545, 43); // moved right to meet the widened (900px) frame; covers the gap left by widening

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

    // Modal popups respond to mouse clicks on their buttons
    m_inputManager->RegisterObserver(m_exitConfirmPopup);
    m_inputManager->RegisterObserver(m_instructionsPopup);
    m_inputManager->RegisterObserver(m_feedbackPopup);
    m_inputManager->RegisterObserver(m_sourcePopup);

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

        if (!optionsMenu->AddCallback("Difficulty", [this](const std::string& choice) { this->OptionsDropDownMenuOnClick(choice); }))
        {
            std::cerr << "Failed to register callback for Difficulty MenuItem" << std::endl;
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

    //Register callbacks for the top-level WordCategoryChoiceMenu (Animals + Eye/Anime/Default)
    if (auto choiceMenu = std::dynamic_pointer_cast<WordCategoryChoiceMenu>(m_choiceMenus["Word Category"]))
    {
        for (auto& category : m_topLevelCategories)
        {
            // "Animals" opens the fly-out instead of loading words; the rest load directly.
            ChoiceMenu::Callback cb;
            if (category == "Animals")
                cb = [this](const std::string&) { this->OpenWordCategoryFlyout("Animals"); };
            else
                cb = [this](const std::string& c) { this->WordCategoryChoiceMenuOnClick(c); };

            if (!choiceMenu->AddCallback(category, cb))
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

    //Register callbacks for the Animals fly-out (each animal is a real category that loads words)
    if (auto flyout = std::dynamic_pointer_cast<WordCategoryChoiceMenu>(m_choiceMenus["Animals"]))
    {
        for (auto& category : m_animalCategories)
        {
            if (!flyout->AddCallback(category, [this](const std::string& c) { this->WordCategoryChoiceMenuOnClick(c); }))
            {
                std::cerr << "Failed to register callback for " << category << " (Animals fly-out)" << std::endl;
                return false;
            }
        }
    }
    else
    {
        std::cerr << "Failed to find Animals fly-out menu" << std::endl;
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

    //Register callbacks for DifficultyChoiceMenu
    if (auto choiceMenu = std::dynamic_pointer_cast<DifficultyChoiceMenu>(m_choiceMenus["Difficulty"]))
    {
        for (auto& option : m_difficultyOptions)
        {
            if (!choiceMenu->AddCallback(option, [this](const std::string& option) { this->DifficultyChoiceMenuOnClick(option); }))
            {
                std::cerr << "Failed to register callback for " << option << " ChoiceMenuItem" << std::endl;
                return false;
            }
        }
    }
    else
    {
        std::cerr << "Failed to find DifficultyChoiceMenu" << std::endl;
        return false;
    }

    // Bind the callback and add the InputTextBox
    m_inputMessageBox->AddInputTextBoxCallback(std::bind(&UIManager::GetUserNamePromptCallback, this));
    m_inputMessageBox->AddButtonCallback(std::bind(&UIManager::GetUserNamePromptCallback, this), InputMessageBox::Buttons::SUBMIT);
    m_inputMessageBox->AddButtonCallback(std::bind(&UIManager::CancelButtonCallback, this), InputMessageBox::Buttons::CANCEL);
    m_inputMessageBox->SetIsActive(true); // ask user for name at start of game

    // --- Exit-confirmation dialog ---
    m_exitConfirmPopup->AddLine("Quit Kaboom Typer?");
    m_exitConfirmPopup->AddLine("Are you sure you want to exit?");
    m_exitConfirmPopup->AddButton("Yes", [this]() { m_inputManager->SetShouldQuit(true); });
    m_exitConfirmPopup->AddButton("No", [this]() { m_exitConfirmPopup->SetIsActive(false); });

    // --- Help: Instructions ---
    const RGBColor keyRed = RGBColor::GetRGBColor(RGBColor::Red);
    const RGBColor white = RGBColor::GetRGBColor(RGBColor::White);
    m_instructionsPopup->AddLine("HOW TO PLAY KABOOM TYPER");
    m_instructionsPopup->AddLine("Type the falling word and press Enter to blow it up.");
    m_instructionsPopup->AddColoredLine({
        { "Type ", white }, { "KABOOM", keyRed }, { " to blow up a block for no points.", white } });
    m_instructionsPopup->AddLine("Clear words before the blocks stack to the top.");
    m_instructionsPopup->AddLine("Arrow keys nudge the active falling block.");
    m_instructionsPopup->AddColoredLine({
        { "F1", keyRed }, { " Start   ", white },
        { "F2", keyRed }, { " Pause   ", white },
        { "F3", keyRed }, { " End Game   ", white },
        { "Esc", keyRed }, { " Menu/Quit", white } });
    m_instructionsPopup->AddButton("Close", [this]() { m_instructionsPopup->SetIsActive(false); });

    // --- Help: Send Feedback ---
    const char* feedbackUrl = "https://www.frankmock.com/software/kaboomtyper/";
    m_feedbackPopup->AddLine("Visit the Kaboom Typer webpage to send feedback:");
    m_feedbackPopup->AddLine(feedbackUrl);
    m_feedbackPopup->AddButton("Open in Browser", [feedbackUrl]() { SDL_OpenURL(feedbackUrl); });
    m_feedbackPopup->AddButton("Close", [this]() { m_feedbackPopup->SetIsActive(false); });

    // --- Help: View Source Code ---
    const char* sourceUrl = "https://github.com/FMock/KaboomTyper";
    m_sourcePopup->AddLine("Visit the GitHub webpage to view the source code:");
    m_sourcePopup->AddLine(sourceUrl);
    m_sourcePopup->AddButton("Open in Browser", [sourceUrl]() { SDL_OpenURL(sourceUrl); });
    m_sourcePopup->AddButton("Close", [this]() { m_sourcePopup->SetIsActive(false); });

    return true;
}

void UIManager::Update(float dt)
{
    m_inputTextBox->Update(dt);
    m_messageBox->Update(dt); // drives the slow flash of the "GAME OVER" banner

    // Tick the score up one point at a time, playing a ding for each, in rapid succession.
    if (m_pendingScoreTicks > 0)
    {
        m_scoreTickTimer += dt;
        if (m_scoreTickTimer >= SCORE_TICK_INTERVAL)
        {
            m_scoreTickTimer -= SCORE_TICK_INTERVAL;
            m_headsUpDisplay->IncreaseScore(1);
            m_headsUpDisplay->SetUpdateRequired(true);
            if (m_scoreDingCallback)
                m_scoreDingCallback();
            --m_pendingScoreTicks;
        }
    }

	if (m_headsUpDisplay->UpdateRequired()) // only update the HUD if needed
		m_headsUpDisplay->Update(dt);

    // Call Update() on each DropDownMenu and keep its top menu-bar button highlighted
    // while that drop-down is open.
    for (auto& pair : m_dropDownMenus)
    {
        if (pair.second)
        {
            pair.second->Update(dt);
            m_gameMenu->SetMenuItemSelected(pair.first, pair.second->GetIsActive());
        }
    }
}

void UIManager::ProcessInput()
{
    std::string submittedStr = Common::GetSubmittedText();

    // Only score during active gameplay. Ignore ENTER used to pick a menu /
    // sub-menu item, and ignore empty submissions (which would spuriously match
    // an empty word before the game starts or after it ends).
    if (Common::CurrentState != GameState::RUNNING || IsAnyMenuOpen() || submittedStr.empty())
        return;

    // "kaboom" (any case) is a cheat: detonate the active block but award no points.
    std::string lowered = submittedStr;
    std::transform(lowered.begin(), lowered.end(), lowered.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (lowered == "kaboom")
    {
        if (m_kaboomCallback)
            m_kaboomCallback(); // GameManager::BlowUpActiveBlock() — no score
        return;
    }

    // Match the typed word against any currently-falling block. The callback (GameManager::UserTyped)
    // destroys the matching block and scores it, returning whether a match was found.
    if (m_wordSubmittedCallback)
        m_wordSubmittedCallback(submittedStr);
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
    std::dynamic_pointer_cast<IDrawable>(m_choiceMenus["Animals"])->SetPriority(9);
    std::dynamic_pointer_cast<IDrawable>(m_choiceMenus["Word Speed"])->SetPriority(9);
    std::dynamic_pointer_cast<IDrawable>(m_choiceMenus["Audio"])->SetPriority(9);
    std::dynamic_pointer_cast<IDrawable>(m_choiceMenus["Difficulty"])->SetPriority(9);
    m_inputMessageBox->SetPriority(13);
    m_exitConfirmPopup->SetPriority(14);
    m_instructionsPopup->SetPriority(14);
    m_feedbackPopup->SetPriority(14);
    m_sourcePopup->SetPriority(14);

    // Sharing IDrawables with DrawOrderManager
    manager.AddDrawable(m_inputTextBox);
    manager.AddDrawable(m_gamePlayArea);
    manager.AddDrawable(m_headsUpDisplay);
    manager.AddDrawable(m_gameMenu);
    manager.AddDrawable(m_messageBox);
    manager.AddDrawable(m_inputMessageBox);
    manager.AddDrawable(m_exitConfirmPopup);
    manager.AddDrawable(m_instructionsPopup);
    manager.AddDrawable(m_feedbackPopup);
    manager.AddDrawable(m_sourcePopup);

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

void GameEngine::UIManager::ShowModal(const std::shared_ptr<PopupMessageBox>& popup)
{
    CloseActiveModal();          // only one popup may be visible at a time
    CloseAllMenus();             // close menus behind it so keyboard input goes to the popup
    popup->SetIsActive(true);
}

void GameEngine::UIManager::ShowExitConfirm()
{
    ShowModal(m_exitConfirmPopup);
}

bool GameEngine::UIManager::IsAnyModalActive() const
{
    return m_exitConfirmPopup->GetIsActive()
        || m_instructionsPopup->GetIsActive()
        || m_feedbackPopup->GetIsActive()
        || m_sourcePopup->GetIsActive();
}

void GameEngine::UIManager::CloseActiveModal()
{
    m_exitConfirmPopup->SetIsActive(false);
    m_instructionsPopup->SetIsActive(false);
    m_feedbackPopup->SetIsActive(false);
    m_sourcePopup->SetIsActive(false);
}

void GameEngine::UIManager::ChangeStartMenuItemLabel(const std::string& newLabelText)
{
    auto menu = std::dynamic_pointer_cast<FileDropDownMenu>(m_dropDownMenus["File"]);
    menu->ChangeMenuItemLabel("Start", newLabelText); // For the menu item with key == "Start", change it's label to newLabelText
}

void GameEngine::UIManager::AddWordSubmittedCallback(WordSubmittedCallback callback)
{
    m_wordSubmittedCallback = callback;
}

void GameEngine::UIManager::AddKaboomCallback(Callback callback)
{
    m_kaboomCallback = callback;
}

void GameEngine::UIManager::AddMenuSelectCallback(Callback callback)
{
    m_menuSelectCallback = callback;
}

void GameEngine::UIManager::AddScoreDingCallback(Callback callback)
{
    m_scoreDingCallback = callback;
}

void GameEngine::UIManager::NotifyMenuSelect()
{
    if (m_menuSelectCallback)
        m_menuSelectCallback();
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

void UIManager::AddDifficultyCallback(DifficultyCallback callback)
{
    m_difficultyCallback = callback;
}

void UIManager::SetDifficultyDisplay(const std::string& text)
{
    m_messageBox->SetDifficulty(text);
}

void UIManager::ResetScore()
{
	m_headsUpDisplay->ResetScore();
	m_pendingScoreTicks = 0; // drop any leftover score-tick cascade from the previous game
	m_scoreTickTimer = 0.0f;
}

void UIManager::IncreaseScore()
{
	// 1 point per character. The just-typed word is still in Common's submitted text here.
	// Queue the points rather than adding them at once: Update() ticks them up one at a
	// time, playing a ding for each so the scoreboard counts up in step with the dings.
	int points = static_cast<int>(Common::GetSubmittedText().length());
	m_pendingScoreTicks += points;
}

void UIManager::GameOver()
{
	// Same single-column hint list as the start screen, with "GAME OVER" shown as a
	// banner in the empty space on the right so all four hint lines still fit.
	m_messageBox->ChangeMessage("F1:       NEW GAME",
								"OPTIONS:  CTRL + O",
								"HELP:     CTRL + H",
								"ESC:      CLOSE MENUS");
	m_messageBox->SetBanner("GAME OVER");
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
    NotifyMenuSelect();
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

std::shared_ptr<GameEngine::ChoiceMenu> GameEngine::UIManager::GetFocusedChoiceMenu() const
{
    // A fly-out (the deepest open menu) takes keyboard focus over its parent.
    auto fly = m_choiceMenus.find("Animals");
    if (fly != m_choiceMenus.end() && fly->second->GetIsActive())
        return fly->second;

    for (const auto& pair : m_choiceMenus)
        if (pair.first != "Animals" && pair.second->GetIsActive())
            return pair.second;

    return nullptr;
}

int GameEngine::UIManager::ComputeFlyoutX(int parentX, int parentWidth, int flyoutWidth) const
{
    // General, off-screen-safe placement: prefer opening to the right of the parent menu; if the
    // window sits so far right that the fly-out would clip off the physical display, open it to
    // the left instead (and vice-versa). Decision is based on the window's on-screen position.
    int winX = 0, winY = 0;
    SDL_Rect disp{ 0, 0, Common::WINDOW_WIDTH, Common::WINDOW_HEIGHT };
    if (SDL_Window* w = SDL_GL_GetCurrentWindow())
    {
        SDL_GetWindowPosition(w, &winX, &winY);
        int d = SDL_GetWindowDisplayIndex(w);
        if (d >= 0)
            SDL_GetDisplayBounds(d, &disp);
    }

    const int rightX = parentX + parentWidth; // fly-out left edge if placed on the right
    const int leftX = parentX - flyoutWidth;  // fly-out left edge if placed on the left

    const bool fitsRight = (winX + rightX + flyoutWidth) <= (disp.x + disp.w);
    const bool fitsLeft = (winX + leftX) >= disp.x;

    if (fitsRight) return rightX;
    if (fitsLeft)  return leftX;

    // Neither side fits fully: choose the side with more room.
    const int roomRight = (disp.x + disp.w) - (winX + rightX);
    const int roomLeft = (winX + parentX) - disp.x;
    return (roomRight >= roomLeft) ? rightX : leftX;
}

void GameEngine::UIManager::OpenWordCategoryFlyout(const std::string& name)
{
    NotifyMenuSelect();
    auto parentIt = m_choiceMenus.find("Word Category");
    auto flyIt = m_choiceMenus.find(name);
    if (parentIt == m_choiceMenus.end() || flyIt == m_choiceMenus.end())
        return;

    auto parent = parentIt->second;
    auto fly = flyIt->second;

    int x = ComputeFlyoutX(parent->GetMenuX(), parent->GetWidth(), fly->GetWidth());
    fly->RepositionX(x);
    fly->SetIsActive(true);
    fly->SetHighlight(0); // give the fly-out keyboard focus on its first item
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

    // 2. Esc: back out one level — fly-out first (focus returns to the Word Category menu),
    //    then any open choice sub-menu (focus returns to the drop-down), then the drop-down;
    //    if nothing is open, defer to the caller (quit).
    if (KeyJustPressed(InputMgr, SDL_SCANCODE_ESCAPE))
    {
        // An open modal popup takes priority: Esc dismisses it (and is treated as
        // "No" / cancel for the exit-confirmation dialog).
        if (IsAnyModalActive())
        {
            CloseActiveModal();
            return true;
        }
        auto flyIt = m_choiceMenus.find("Animals");
        if (flyIt != m_choiceMenus.end() && flyIt->second->GetIsActive())
        {
            flyIt->second->SetIsActive(false);
            flyIt->second->ResetHighlight();
            return true;
        }
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
        // Nothing else open: prompt for exit instead of quitting outright.
        ShowExitConfirm();
        return true;
    }

    // 3a. Arrow / Enter navigation while a choice sub-menu has focus (a fly-out wins over its
    //     parent). Right descends into a fly-out; Left backs out of one.
    if (auto focus = GetFocusedChoiceMenu())
    {
        auto flyIt = m_choiceMenus.find("Animals");
        const bool focusIsFlyout = (flyIt != m_choiceMenus.end() && focus == flyIt->second);

        if (KeyJustPressed(InputMgr, SDL_SCANCODE_DOWN))
        {
            focus->MoveHighlight(1);
            return true;
        }
        if (KeyJustPressed(InputMgr, SDL_SCANCODE_UP))
        {
            focus->MoveHighlight(-1);
            return true;
        }
        if (KeyJustPressed(InputMgr, SDL_SCANCODE_RIGHT))
        {
            // Descend into a fly-out when the highlighted item opens one.
            if (!focusIsFlyout && focus->GetHighlightedName() == "Animals")
                OpenWordCategoryFlyout("Animals");
            return true;
        }
        if (KeyJustPressed(InputMgr, SDL_SCANCODE_LEFT))
        {
            // Back out of a fly-out to its parent menu.
            if (focusIsFlyout)
            {
                focus->SetIsActive(false);
                focus->ResetHighlight();
            }
            return true;
        }
        if (KeyJustPressed(InputMgr, SDL_SCANCODE_RETURN) || KeyJustPressed(InputMgr, SDL_SCANCODE_KP_ENTER))
        {
            focus->ActivateHighlighted(); // select the category/speed/audio option (or open the fly-out)
            // Selecting "Animals" in the parent opens the fly-out — move focus into it.
            if (!focusIsFlyout && flyIt != m_choiceMenus.end() && flyIt->second->GetIsActive())
                flyIt->second->SetHighlight(0);
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
    NotifyMenuSelect();
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
        ShowExitConfirm(); // confirm before quitting
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
    NotifyMenuSelect();
    // The Animals fly-out is a child of the Word Category menu; reset it whenever the user
    // switches Options sub-menus so it can't be left visible without its parent.
    if (auto fly = m_choiceMenus.find("Animals"); fly != m_choiceMenus.end())
    {
        fly->second->SetIsActive(false);
        fly->second->ResetHighlight();
    }

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
            m_choiceMenus["Difficulty"]->SetIsActive(false);
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
            m_choiceMenus["Difficulty"]->SetIsActive(false);
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
            m_choiceMenus["Difficulty"]->SetIsActive(false);
        }
    }
    else if (choice == "DIFFICULTY")
    {
#if DEBUG
        std::cout << "Display Difficulty Options" << std::endl;
#endif
        m_choiceMenus["Difficulty"]->SetIsActive(!m_choiceMenus["Difficulty"]->GetIsActive());
        if (m_choiceMenus["Difficulty"]->GetIsActive())
        {
            m_choiceMenus["Word Category"]->SetIsActive(false);
            m_choiceMenus["Audio"]->SetIsActive(false);
            m_choiceMenus["Word Speed"]->SetIsActive(false);
        }
    }
    else
    {
        // Do Nothing
    }
}


void GameEngine::UIManager::WordCategoryChoiceMenuOnClick(const std::string& choice)
{
    NotifyMenuSelect();
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
    NotifyMenuSelect();
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
    NotifyMenuSelect();
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

void GameEngine::UIManager::DifficultyChoiceMenuOnClick(const std::string& choice)
{
    NotifyMenuSelect();
    bool state = false;
    std::string selection = choice;

    for (const auto& option : m_difficultyOptions)
    {
        if (option == choice) // valid difficulty option
        {
            auto menu = std::dynamic_pointer_cast<DifficultyChoiceMenu>(m_choiceMenus["Difficulty"]);
            state = menu->GetMenuItemSelectionState(option);
        }
    }

    // If the player toggled the current difficulty off, fall back to the default (Normal).
    if (!state)
    {
        selection = "Normal";
    }

    if (m_difficultyCallback)
        m_difficultyCallback(selection);
}

void GameEngine::UIManager::HelpDropDownMenuOnClick(const std::string& choice)
{
    NotifyMenuSelect();
    if (choice == "INSTRUCTIONS")
    {
#if DEBUG
        std::cout << "Display Instructions" << std::endl;
#endif
        ShowModal(m_instructionsPopup);
    }
    else if (choice == "VIEW SOURCE CODE")
    {
#if DEBUG
        std::cout << "Display View Source Code Option" << std::endl;
#endif
        ShowModal(m_sourcePopup);
    }
    else if (choice == "SEND FEEDBACK")
    {
#if DEBUG
        std::cout << "Display Send Feedback Option" << std::endl;
#endif
        ShowModal(m_feedbackPopup);
    }
    else
    {
        // Do Nothing
    }
}