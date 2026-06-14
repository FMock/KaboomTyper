#pragma once

#include "IDrawable.h"
#include "Color.h"
#include "RGBColor.h"
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
		// Opt-in: draw a colored outline frame around this rectangle (in addition to the
		// fill when filled). Rectangles that never call this are drawn exactly as before.
		void SetFrameColor(const RGBColor& color) { m_frameColor = color; m_hasFrame = true; }
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
		RGBColor m_frameColor{ 255, 255, 255 }; // outline color when m_hasFrame
		bool m_hasFrame = false;                // opt-in colored outline (see SetFrameColor)
	};
}