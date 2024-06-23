#pragma once

#include "SideBar.h"
#include "IDrawable.h"

namespace GameEngine
{
	class GamePlayArea : public IDrawable
	{
	public:
		GamePlayArea() = default;
		~GamePlayArea() = default;
		void Initialize();
		void Render();
		void Draw() override;
		void Update(float dt) override;
		int GetPriority() const override { return m_priority; }
		void SetPriority(int priority) override { m_priority = priority; }

	private:
		SideBar m_leftSideBar;
		SideBar m_rightSideBar;
		SideBar m_topSideBar;
		SideBar m_bottomSideBar;
		int m_priority = 0;; // draw priority
	};
}
