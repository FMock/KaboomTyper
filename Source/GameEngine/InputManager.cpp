#include "InputManager.h"
#include "Common.h"
#include <cstring>
#include <iostream>
#include <string>

using namespace GameEngine;

GameEngine::InputManager::InputManager()
{
	m_kbState = SDL_GetKeyboardState(NULL); // initialize the keyboard state tracker
}

void GameEngine::InputManager::PrepareForStateChange()
{
	std::memcpy(m_kbPrevState, m_kbState, sizeof(m_kbPrevState)); // copy key states into previous key states array

	// Update previous mouse button state for the next frame
	std::copy(std::begin(m_mouseButtonState), std::end(m_mouseButtonState), std::begin(m_prevMouseButtonState));
}

void GameEngine::InputManager::GetNewInputState()
{
	m_kbState = SDL_GetKeyboardState(NULL);// We want new status of all the keys

	while (SDL_PollEvent(&m_event))
	{
		switch (m_event.type)
		{
		case SDL_QUIT:
			m_quit = 1;
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			// Update mouse button states
			UpdateMouseButtonState(m_event.button.button, m_event.type == SDL_MOUSEBUTTONDOWN);
			break;
		}
	}
}

void InputManager::GetMousePosition(int* x, int* y)
{
	SDL_GetMouseState(x, y);
	// The window is enlarged by BORDER_THICKNESS on every side and content is inset by that much
	// (see Game::Initialize), so convert the physical mouse position back to logical coordinates.
	*x -= Common::BORDER_THICKNESS;
	*y -= Common::BORDER_THICKNESS;
}

void InputManager::UpdateMouseButtonState(int button, bool state)
{
	if (button >= 1 && button <= SDL_BUTTON_X2)
	{
		m_prevMouseButtonState[button - 1] = m_mouseButtonState[button - 1];
		m_mouseButtonState[button - 1] = state;
	}
}

void GameEngine::InputManager::Update()
{
	PrepareForStateChange(); // copy previous state first
	GetNewInputState();      // next get new input state
	NotifyObservers();       // then notify observers
}

bool GameEngine::InputManager::ShouldQuit() const
{
	return m_quit;
}

void GameEngine::InputManager::SetShouldQuit(bool shouldQuit)
{
    m_quit = shouldQuit;
}

void InputManager::RegisterObserver(std::shared_ptr<InputObserver> observer)
{
    m_myObservers.push_back(observer);
}

void InputManager::UnregisterObserver(InputObserver* observer)
{
    m_myObservers.erase(std::remove_if(m_myObservers.begin(), m_myObservers.end(),
        [observer](const std::weak_ptr<InputObserver>& o)
        {
            if (auto shared = o.lock())
            {
                return shared.get() == observer;
            }
            return false;
        }), m_myObservers.end());
}

void InputManager::NotifyObservers()
{
    auto observersCopy = m_myObservers;

    for (const auto& weakObserver : observersCopy)
    {
        if (auto observer = weakObserver.lock())
        {
            try
            {
                observer->Notify(this);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Exception caught during Notify: " << e.what() << std::endl;
            }
        }
        else
        {
            std::cerr << "Warning: Null observer in the list" << std::endl;
        }
    }

    // Remove expired weak pointers
    m_myObservers.erase(std::remove_if(m_myObservers.begin(), m_myObservers.end(),
        [](const std::weak_ptr<InputObserver>& o)
        {
            return o.expired();
        }), m_myObservers.end());
}
