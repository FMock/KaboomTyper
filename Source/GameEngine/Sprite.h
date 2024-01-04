#pragma once

#include "InputObserver.h"
#include "InputManager.h"
#include <vector>
#include <string>


namespace GameEngine
{
	enum class MoveDirection
	{
		NONE = -1, LEFT, RIGHT, UP, DOWN
	};

	class Sprite : public InputObserver
	{
	public:
		Sprite();
		void Update();
		void ProcessInput();
		virtual void RespondToObserved(InputManager* InputMgr);
	private:
		MoveDirection m_moveDirection;
		std::vector<std::string> m_move;
	};
}