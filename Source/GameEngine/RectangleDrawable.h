#pragma once

#include "IDrawable.h"
#include "Colors.h"

namespace GameEngine
{
	class RectangleDrawable : public IDrawable
	{
	public:
		RectangleDrawable();
		RectangleDrawable(int x, int y, int width, int height, Colors rectColor = DEFAULT_COLOR);
		~RectangleDrawable();
		void Initialize(int x, int y, int width, int height, Colors rectColor = DEFAULT_COLOR);
		void Update() override;
		void Draw() override;
		void SetWidth(int width);
		int GetWidth() const;
		void SetHeight(int height);
		int GetHeight() const;

	private:
		void DrawRectangle();
		int m_width;
		int m_height;
		bool m_initialized;
	};
}