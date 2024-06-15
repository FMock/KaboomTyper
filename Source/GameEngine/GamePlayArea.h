#pragma once

#include "SideBar.h"

namespace GameEngine
{
	class GamePlayArea
	{
	public:
		GamePlayArea() = default;
		~GamePlayArea() = default;
		void Initialize();
		void Render();

	private:
		SideBar m_leftSideBar;
		SideBar m_rightSideBar;
		SideBar m_topSideBar;
		SideBar m_bottomSideBar;
	};
}
