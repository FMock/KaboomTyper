#include "Menu.h"
#include "GlobalPreprocessorFlags.h"
#include <iostream>

using namespace GameEngine;

Menu::Menu()
    : m_priority(0)
{
    InitializeCommonElements();
}

void Menu::Draw()
{
    m_menuBar->Draw();
    m_title.DrawText(1.0f);
    m_footer->Draw();

    // Draw all buttons
    for (const auto& item : m_menuItems)
    {
        if (item.second.button)
        {
#if DEBUG_MENU
            std::cout << "Drawing button: " << item.first << std::endl;
#endif
            item.second.button->Draw();
        }
        else
        {
            std::cerr << "Error: Null button pointer for item: " << item.first << std::endl;
        }
    }
}

void Menu::Update(float dt)
{
    // Update all buttons
    for (auto& item : m_menuItems)
    {
        if (item.second.button)
        {
            item.second.button->Update(dt);
        }
    }
}

void Menu::AddMenuItem(const std::string& name, std::unique_ptr<Button> button, Callback callback)
{
#if DEBUG_MENU
    std::cout << "Adding menu item: " << name << std::endl;
#endif

    if (!button)
    {
        std::cerr << "Error: Null button passed for menu item: " << name << std::endl;
    }
    m_menuItems[name] = MenuEntry{ std::move(button), callback };
}

bool Menu::AddCallback(const std::string& name, Callback callback)
{
    auto it = m_menuItems.find(name);
    if (it != m_menuItems.end())
    {
        it->second.callback = callback;
        return true;
    }
#if DEBUG_MENU
    std::cout << "Warning: Attempting to add callback to non-existent menu item: " << name << std::endl;
#endif
    return false;
}

void Menu::InitializeMenu(const std::string& name, int nameX, int nameY, int x, int y, int width, int height, GameEngine::Colors, bool fill)
{
    m_menuBar->Initialize(x, y, width, height, Colors::BLUE, fill);
    m_title = TextString(name, nameX, nameY);

    // Initialize menu items
    for (auto& item : m_menuItems)
    {
        InitializeMenuItem(item.second.button.get(), item.first, 0, 0, 1.0f, Colors::DEFAULT_COLOR); // Pass default values
    }

    m_footer->Initialize(0, 950, 900, 42, Colors::BLUE, true);// TODO: REMOVE FOOTER FROM MENU
}

void Menu::RespondToObserved(InputManager* InputMgr)
{
    for (auto& item : m_menuItems)
    {
        HandleButton(InputMgr, item.second.button.get(), item.first, item.second.callback);
    }
}

void Menu::HandleButton(InputManager* InputMgr, Button* button, const std::string& buttonName, Callback callback)
{
    int mouseX, mouseY;
    InputMgr->GetMousePosition(&mouseX, &mouseY);

    button->SetIsActive(button->IsMouseOverButton(mouseX, mouseY)); // toggle current Button active state

    if (InputMgr->m_mouseButtonState[0] && !InputMgr->m_prevMouseButtonState[0] && button->IsMouseOverButton(mouseX, mouseY))
    {
#if DEBUG_MENU
        std::cout << buttonName << " button clicked" << std::endl;
#endif
        button->SetButtonColor(Colors::DARK_GRAY);
        callback(buttonName);
    }
    else if (!InputMgr->m_mouseButtonState[0] && InputMgr->m_prevMouseButtonState[0] && button->IsMouseOverButton(mouseX, mouseY))
    {
#if DEBUG_MENU
        std::cout << buttonName << " button released" << std::endl;
#endif
        button->SetButtonColor(Colors::DEFAULT_COLOR);
    }
}

void Menu::SetMenuItemSelected(const std::string& name, bool selected)
{
    auto it = m_menuItems.find(name);
    if (it != m_menuItems.end() && it->second.button)
        it->second.button->SetSelected(selected);
}

bool Menu::IsPointInMenuBar(int x, int y) const
{
    if (!m_menuBar)
        return false;

    int left = m_menuBar->GetXPosition();
    int top = m_menuBar->GetYPosition();
    return x >= left && x <= left + m_menuBar->GetWidth() &&
           y >= top && y <= top + m_menuBar->GetHeight();
}

void Menu::InitializeCommonElements()
{
    m_menuBar = std::make_unique<RectangleDrawable>();
    m_footer = std::make_unique<RectangleDrawable>();
}
