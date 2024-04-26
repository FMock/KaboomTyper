#pragma once

#include "IDrawable.h"
#include "Colors.h"
#include <memory>
#include <GL/glew.h>

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

#pragma region setters_and_getters
		void SetWidth(int width);
		int GetWidth() const;
		void SetHeight(int height);
		int GetHeight() const;
#pragma endregion

	private:
		void DrawRectangle();
		int m_width;
		int m_height;
		int m_colorTextureWidth;
		int m_colorTextureHeight;
		GLuint m_colorTexture;
		bool m_initialized;
	};
}