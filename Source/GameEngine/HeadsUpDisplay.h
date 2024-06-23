#pragma once

#include "RectangleDrawable.h"
#include "IDrawable.h"
#include "TextString.h"
#include "Score.h"
#include <memory>

namespace GameEngine
{
	class HeadsUpDisplay : public IDrawable
	{
	public:
		HeadsUpDisplay();
		~HeadsUpDisplay();

		void Draw() override;
		void Update(float dt) override;
		void Initialize(int x, int y);
		Score* GetScore() const; // share the score with rest of game
		void IncreaseScore(int amount);
		void SetUpdateRequired(bool updateRequired);
		bool UpdateRequired();
		void ResetScore();
		int GetPriority() const override { return m_priority; }
		void SetPriority(int priority) override { m_priority = priority; }

	private:
		void DrawHud();
		void UpdateScore();
		std::unique_ptr<RectangleDrawable> m_background;
		std::unique_ptr<TextString> m_scoreLabel;
		std::unique_ptr<TextString> m_scoreAsText;
		std::unique_ptr<TextString> m_highScoreLabel;
		std::unique_ptr<TextString> m_highScoreAsText;
		std::unique_ptr<TextString> m_levelLabel;
		std::unique_ptr<TextString> m_levelAsText;
		std::unique_ptr<Score> m_score;

		int m_x, m_y; // HUD Position
		int m_width, m_height; // HUD Width and Height
		bool m_updatedRequired;
		int m_column2XOffset;
		int m_xPad;
		int m_yPad;
		int m_priority = 1; // draw priority
	};
}