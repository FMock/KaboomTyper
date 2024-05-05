#pragma once

#include "InputObserver.h"
#include "RectangleDrawable.h"
#include "TextString.h"
#include "Colors.h"
#include <memory>
#include <string>

namespace GameEngine
{
	class Button : public InputObserver
	{
	public:
		Button();
		Button(std::string text, int x, int y, float scale = 1.0f, Colors color = Colors::DEFAULT_COLOR);
		~Button();
		void Initialize(std::string text, int x, int y, float scale = 1.0f, Colors color = Colors::DEFAULT_COLOR);
		void Update(float dt);
		void Draw();

	private:
		std::unique_ptr<RectangleDrawable> m_body;
		std::unique_ptr<TextString> m_label;
		int m_adjustedWidth;
		float m_scaler;

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}