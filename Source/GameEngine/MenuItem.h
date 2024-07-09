#pragma once

#include "RectangleDrawable.h"
#include "TextString.h"
#include "Color.h"
#include <memory>
#include <string>

namespace GameEngine
{
	class MenuItem
	{
	public:
		MenuItem();
		~MenuItem();
		MenuItem(std::string text, int x, int y, int parentX, int parentY, int parentWidth, float scale = 1.0f, Colors color = Colors::DEFAULT_COLOR);
		void Initialize(std::string text, int x, int y, int parentX, int parentY, int parentWidth, float scale = 1.0f, Colors color = Colors::DEFAULT_COLOR);
		void Update(float dt);
		void Draw();
		bool IsMouseOverMenuItem(const int& x, const int& y);
		void SetMenuItemColor(Colors color);
		int GetXPosition() const;
		int GetYPostion() const;
		int GetWidth() const;
		int GetHeight() const;
		bool GetIsActive() const;
		void SetIsActive(bool isActive);

	private:
		std::unique_ptr<RectangleDrawable> m_outline;
		int m_outlineWidth;
		int m_outlineHeight;
		std::unique_ptr<TextString> m_label;
		int m_adjustedWidth;
		float m_scaler;
		int m_xPos;
		int m_yPos;
		int m_width;
		int m_height;
		bool m_active; // true when mouse hover present
	};
}