#pragma once

#include "RectangleDrawable.h"

namespace GameEngine
{
	class SideBar : public RectangleDrawable
	{
	public:
		using RectangleDrawable::RectangleDrawable; // Inherit constructors

		void Initialize(int x, int y, int width, int height, Colors rectColor, bool filled) override;
		void Update(float dt) override;
		void Draw() override;
	};
}