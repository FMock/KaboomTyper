#pragma once

#include "RectangleDrawable.h"
#include "TextString.h"
#include "Score.h"
#include <memory>

namespace GameEngine
{
	class HeadsUpDisplay
	{
	public:
		HeadsUpDisplay();
		~HeadsUpDisplay();
		void Draw();
		void Initialize(int x, int y);
		Score* GetScore() const; // share the score with rest of game

	private:
		void DrawHud();
		std::unique_ptr<RectangleDrawable> m_background;
		std::unique_ptr<TextString> m_scoreLabel;
		std::unique_ptr<TextString> m_scoreAsText;
		std::unique_ptr<Score> m_score;
		int m_x, m_y;
		
	};
}