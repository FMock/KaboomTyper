#pragma once

namespace GameEngine
{
	class InputManager; // forward declaration

	class InputObserver
	{
	public:
		inline InputObserver(void) {}
		inline ~InputObserver(void) {}
		virtual void RespondToObserved(InputManager*) = 0;
		template<typename T> void Notify(T*);
	};

	/* InputObserver receives notification from InputManager) */
	template<typename T> inline void InputObserver::Notify(T* type)
	{
		RespondToObserved(type);
	}
}