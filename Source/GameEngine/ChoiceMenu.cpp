#include "ChoiceMenu.h"
#include "Common.h"

GameEngine::ChoiceMenu::ChoiceMenu() : m_isActive(false), m_priority(0), m_count(0)
{
	SetIsActive(false);
	InitializeCommonElements();
}

GameEngine::ChoiceMenu::~ChoiceMenu() = default;

void GameEngine::ChoiceMenu::Draw()
{
	if (GetIsActive())
	{
		m_menuBody->Draw();

		// Draw all menu items
		for (const auto& item : m_choiceMenuItems)
		{
			if (item.second.menuItem)
			{
#if DEBUG_CHOICEMENU
				//std::cout << "Drawing ChoiceMenuItem: " << item.first << std::endl;
#endif
				item.second.menuItem->Draw();
			}
			else
			{
				std::cerr << "Error: Null choice menu item pointer for item: " << item.first << std::endl;
			}
		}
	}
}

void GameEngine::ChoiceMenu::Update(float dt)
{
	// Update all MenuItems
	for (auto& item : m_choiceMenuItems)
	{
		if (item.second.menuItem)
		{
			item.second.menuItem->Update(dt);
		}
	}
}


void GameEngine::ChoiceMenu::AddChoiceMenuItem(const std::string& name, std::unique_ptr<ChoiceMenuItem> choiceMenuItem, Callback callback)
{
	if (!choiceMenuItem)
	{
		std::cerr << "Error: Null ChoiceMenuItem passed for choiceMenuItem: " << name << std::endl;
		return; // Early exit if choiceMenuItem is null
	}

	m_choiceMenuItems[name] = ChoiceMenuEntry{ std::move(choiceMenuItem), std::move(callback) };
	m_choiceMenuItems[name].menuItem.get()->SetName(name);
}


bool GameEngine::ChoiceMenu::AddCallback(const std::string& name, Callback callback)
{
	auto it = m_choiceMenuItems.find(name);
	if (it != m_choiceMenuItems.end())
	{
		it->second.callback = callback;
		return true;
	}
#if DEBUG_CHOICEMENU
	std::cout << "Warning: Attempting to add callback to non-existent choice menu item: " << name << std::endl;
#endif
	return false;
}

void GameEngine::ChoiceMenu::InitializeChoiceMenu(int x, int y, int width, int height, Colors, bool)
{
	m_menuBody->Initialize(x, y, width, height, Colors::BLUE, true);

	int verticalSpacer = 32;

	// Determine the largest string size
	size_t max_size = 0;
	for (const auto& item : m_choiceMenuItems)
	{
		if (item.first.size() > max_size)
		{
			max_size = item.first.size();
		}
	}

	// Initialize choice menu items
	for (auto& item : m_choiceMenuItems)
	{

		// Determine the number of asterisks to append
		int augment_size = static_cast<int>(max_size) - static_cast<int>(item.first.size());
		item.second.menuItem.get()->SetAugmentedSize(augment_size);


		InitializeMenuEntry(item.second.menuItem.get(), item.first, x, y + verticalSpacer, 1.0f, Colors::DEFAULT_COLOR); // Pass default values
		item.second.menuItem.get()->SetXPosition(x);
		item.second.menuItem.get()->SetYPosition(y + verticalSpacer);
		auto scaler = item.second.menuItem.get()->GetLableScaler();
		item.second.menuItem.get()->AugmentLabel(augment_size);
		y += (Common::FONT_HEIGHT * 1.25);
	}
}


bool GameEngine::ChoiceMenu::GetIsActive() const
{
	return m_isActive;
}

void GameEngine::ChoiceMenu::SetIsActive(bool isActive)
{
	m_isActive = isActive;
}

void GameEngine::ChoiceMenu::RespondToObserved(InputManager* InputMgr)
{
	if (GetIsActive())
	{
		for (auto& item : m_choiceMenuItems)
		{
			HandleMenuItem(InputMgr, item.second.menuItem.get(), item.first, item.second.callback);
		}
	}
}

void GameEngine::ChoiceMenu::HandleMenuItem(InputManager* InputMgr, ChoiceMenuItem* menuItem, const std::string& menuItemName, Callback callback)
{
	int mouseX, mouseY;
	InputMgr->GetMousePosition(&mouseX, &mouseY);

	menuItem->SetIsActive(menuItem->MouseHoverPresent(mouseX, mouseY)); // toggle current menuItem active state

	if (InputMgr->m_mouseButtonState[0] && !InputMgr->m_prevMouseButtonState[0] && menuItem->MouseHoverPresent(mouseX, mouseY))
	{
#if DEBUG_CHOICEMENU
		std::cout << menuItem << " ChoiceMenuItem clicked" << std::endl;
#endif
		menuItem->SetIsSelected(!menuItem->GetIsSelected());
		callback(menuItemName);
	}
	else if (!InputMgr->m_mouseButtonState[0] && InputMgr->m_prevMouseButtonState[0] && menuItem->MouseHoverPresent(mouseX, mouseY))
	{
#if DEBUG_CHOICEMENU
		std::cout << menuItem << " ChoiceMenuItem released" << std::endl;
#endif
	}
}

void GameEngine::ChoiceMenu::InitializeCommonElements()
{
	m_menuBody = std::make_unique<RectangleDrawable>();
}
