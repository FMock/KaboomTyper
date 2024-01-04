#pragma once
#include <SDL.h>
#include <vector>
#include <map>
#include "InputObserver.h"

namespace GameEngine
{
	class InputManager
	{
	public:
		InputManager();
		void ProcessInput();
		bool ShouldQuit() const;
		bool m_quit = 0;
		void NotifyObservers();
		void RegisterObserver(InputObserver*);
		void UnregisterObserver(InputObserver*);

		// The previous frame's keyboard state.
		unsigned char m_kbPrevState[SDL_NUM_SCANCODES] = { 0 };

		// The current frame's keyboard state.
		const unsigned char* m_kbState = NULL;

	private:
		void PrepareForStateChange();
		void GetNewInputState();
		SDL_Event m_event;
		std::vector<InputObserver*> m_myObservers;
	};
}