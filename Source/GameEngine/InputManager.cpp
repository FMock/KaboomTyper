#include "InputManager.h"
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

void GameEngine::InputManager::NotifyObservers()
{
	for (auto observer : m_myObservers)
	{
		observer->Notify(this);
	}
}

void GameEngine::InputManager::RegisterObserver(InputObserver* observer)
{
	m_myObservers.push_back(observer);
}

void GameEngine::InputManager::UnregisterObserver(InputObserver* observer)
{
	for (unsigned int i = 0; i < m_myObservers.size(); i++)
	{
		if (m_myObservers.at(i) == observer)
		{
			m_myObservers.erase(m_myObservers.begin() + i);
			return;
		}
	}
}
