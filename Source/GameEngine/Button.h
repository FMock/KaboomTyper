#pragma once

#include "RectangleDrawable.h"
#include "TextString.h"
#include "Colors.h"
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
		void ButtonReleased();

	private:
		std::unique_ptr<RectangleDrawable> m_body;
		std::unique_ptr<TextString> m_label;
		int m_adjustedWidth;
		float m_scaler;

	};
}