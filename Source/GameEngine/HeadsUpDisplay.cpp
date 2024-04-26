#include "HeadsUpDisplay.h"

using namespace GameEngine;

HeadsUpDisplay::HeadsUpDisplay() 
	: m_background(std::make_unique<RectangleDrawable>()), 
	  m_scoreLabel(std::make_unique<TextString>()),
	  m_scoreAsText(std::make_unique<TextString>()),
	  m_score(std::make_unique<Score>()),
	  m_x(0), m_y(0)
{
}

HeadsUpDisplay::~HeadsUpDisplay()
{
}

void HeadsUpDisplay::Draw()
{
	DrawHud();
}

void GameEngine::HeadsUpDisplay::Initialize(int x, int y)
{
	m_x = x;
	m_y = y;
	m_background->Initialize(m_x, m_y, 325, 300, Colors::DARK_YELLOW);
	m_scoreLabel->Initialize("SCORE:", m_x + 5, m_y + 5); // draw some text to the screen

	// Retrieve the score as an integer
	int scoreValue = m_score->GetScore();

	// Ensure the score is limited to 7 digits
	if (scoreValue > 9999999)
	{
		scoreValue = 9999999;
	}
	// Convert the integer score to a string with leading zeros and a width of 7 characters
	std::string score = std::to_string(scoreValue);
	score = std::string(7 - score.length(), '0') + score;

	m_scoreAsText->Initialize(score, m_x + 150, m_y + 5);

}

void HeadsUpDisplay::DrawHud()
{
	m_background->Draw();
	m_scoreLabel->DrawText();
	m_scoreAsText->DrawText();
}

