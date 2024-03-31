#pragma once

namespace GameEngine
{
	class InputManager; // forward declaration

	class InputObserver
	{
	public:
		InputObserver(void) {}
		~InputObserver(void) {}
		template<typename T> void Notify(T*);

	protected:
		virtual void RespondToObserved(InputManager*) = 0;
	};

	/* InputObserver receives notification from InputManager) */
	template<typename T> inline void InputObserver::Notify(T* type)
	{
		RespondToObserved(type);
	}
}