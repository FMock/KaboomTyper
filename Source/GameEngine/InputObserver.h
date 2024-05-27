#pragma once

namespace GameEngine
{
	class InputManager; // forward declaration to minimize dependencies

	class InputObserver
	{
	public:
		InputObserver(void) = default;
		virtual ~InputObserver(void) = default;
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

