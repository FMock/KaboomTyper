#pragma once
#include <SDL.h>
#include <vector>
#include <map>
#include "InputObserver.h"
#include <memory>

namespace GameEngine
{
	class InputManager
	{
	public:
		InputManager();
		void Update();
		bool ShouldQuit() const;
		bool m_quit = 0;
		void NotifyObservers();
		void RegisterObserver(std::shared_ptr<InputObserver> observer);
		void UnregisterObserver(InputObserver*);

		// The previous frame's keyboard state.
		unsigned char m_kbPrevState[SDL_NUM_SCANCODES] = { 0 };

		bool m_mouseButtonState[3]; // Array to track mouse button states
		bool m_prevMouseButtonState[3]; // Array to track previous mouse button states
		void GetMousePosition(int* x, int* y);

		void UpdateMouseButtonState(int button, bool state);

		// The current frame's keyboard state.
		const unsigned char* m_kbState = NULL;

	private:
		void PrepareForStateChange();
		void GetNewInputState();
		SDL_Event m_event;
		std::vector<std::weak_ptr<InputObserver>> m_myObservers;
	};
}