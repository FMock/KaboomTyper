#pragma once

#include "IDrawable.h"

namespace GameEngine
{
	class RectangleDrawable : public IDrawable
	{
	public:
		RectangleDrawable();
		~RectangleDrawable();
		void Update() override;
		void Draw() override;
	private:
		void DrawRectangle();
	};
}