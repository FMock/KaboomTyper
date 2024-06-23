#pragma once

#include "RectangleDrawable.h"
#include "TextString.h"
#include "Color.h"
#include <memory>
#include <string>

namespace GameEngine
{
	class Button
	{
	public:
		Button();
		Button(std::string text, int x, int y, float scale = 1.0f, Colors color = Colors::DEFAULT_COLOR);
		~Button();
		void Initialize(std::string text, int x, int y, float scale = 1.0f, Colors color = Colors::DEFAULT_COLOR);
		void Update(float dt);
		void Draw();
		void ButtonPressed();
		bool IsMouseOverButton(const int& x, const int& y);
		void ButtonReleased();
		void SetButtonColor(Colors color);
		int GetXPosition() const;
		int GetYPostion() const;
		int GetWidth() const;
		int GetHeight() const;
		void SetIsActive(bool isActive);

	private:
		std::unique_ptr<RectangleDrawable> m_outline;
		std::unique_ptr<TextString> m_label;
		int m_adjustedWidth;
		float m_scaler;
		int m_xPos;
		int m_yPos;
		int m_width;
		int m_height;
		int m_outlineWidth;
		int m_outlineHeight;
		bool m_active; // true when mouse hover present
	};
}