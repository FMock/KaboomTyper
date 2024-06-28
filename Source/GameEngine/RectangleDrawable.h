#pragma once

#include "IDrawable.h"
#include "Color.h"
#include <memory>
#include <GL/glew.h>

namespace GameEngine
{
	class RectangleDrawable : public IDrawable
	{
	private:
		int m_priority = 0; // draw priority;

	public:
		RectangleDrawable();
		RectangleDrawable(int x, int y, int width, int height, Colors rectColor = DEFAULT_COLOR, bool filled = false);
		virtual ~RectangleDrawable() = default;
		virtual void Initialize(int x, int y, int width, int height, Colors rectColor = DEFAULT_COLOR, bool filled = false);
		virtual void Update(float dt) override;
		void Draw() override;
		void ChangeColor(Colors color);
		int GetPriority() const override { return m_priority; }
		void SetPriority(int priority) override { m_priority = priority; }
#pragma region setters_and_getters
		void SetWidth(int width);
		int GetWidth() const;
		void SetHeight(int height);
		int GetHeight() const;
		void SetXPos(int x);
		int GetXPosition() const;
		int GetYPosition() const;
#pragma endregion

	protected:
		void DrawRectangle();
		int m_width;
		int m_height;
		int m_colorTextureWidth;
		int m_colorTextureHeight;
		GLuint m_colorTexture;
		bool m_filled;
		bool m_initialized;
	};
}