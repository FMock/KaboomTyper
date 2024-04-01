#pragma once

#include "Common.h"

namespace GameEngine
{
	class Moveable
	{
	public:
		Moveable();
		float m_changeX, m_changeY, m_speedX, m_speedY;
		bool m_isMoving;
		void MoveRight();
		void MoveLeft();
		void MoveUp();
		void MoveDown();
		void Stop();
		MoveDirection m_moveDirection;
	};
}