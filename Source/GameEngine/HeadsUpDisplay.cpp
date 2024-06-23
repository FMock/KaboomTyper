#include "HeadsUpDisplay.h"

using namespace GameEngine;

HeadsUpDisplay::HeadsUpDisplay() 
	: m_background(std::make_unique<RectangleDrawable>()),
	  m_levelLabel(std::make_unique<TextString>()),
	  m_levelAsText(std::make_unique<TextString>()),
	  m_scoreLabel(std::make_unique<TextString>()),
	  m_scoreAsText(std::make_unique<TextString>()),
	  m_highScoreLabel(std::make_unique<TextString>()),
	  m_highScoreAsText(std::make_unique<TextString>()),
	  m_score(std::make_unique<Score>()),
	  m_x(0), m_y(0), m_width(0), m_height(0), m_updatedRequired(false),
	  m_priority(0)
{

}

HeadsUpDisplay::~HeadsUpDisplay()
{
}

void HeadsUpDisplay::Draw()
{
	DrawHud();
}

void GameEngine::HeadsUpDisplay::Update(float dt)
{
	UpdateScore();
	m_updatedRequired = false;
}

void GameEngine::HeadsUpDisplay::Initialize(int x, int y)
{
	/****** HUD Background ******/
	m_x = x;
	m_y = y;
	m_width = 345; // TODO: fix. magic number
	m_height = 104; // TODO: fix. magic number
	m_xPad = 10; // TODO: fix. magic number
	m_yPad = 5; // TODO: fix. magic number
	m_column2XOffset = 165;// TODO: fix. magic number

	m_background->Initialize(m_x, m_y, m_width, m_height, Colors::RED, true); // true because we want to rectangle filled in

	/****** Level  ******/
	m_levelLabel->Initialize("LEVEL:", m_x + m_xPad, m_y + m_yPad);

	int levelValue = 1;

	// Ensure the score is limited to 7 digits
	if (levelValue > 9999999)
		levelValue = 9999999;

	// Convert the integer score to a string with leading zeros and a width of 7 characters
	std::string level = std::to_string(levelValue);
	level = std::string(7 - level.length(), '0') + level;

	m_levelAsText->Initialize(level, m_x + m_column2XOffset, m_y + 5);

	/****** Score ******/
	m_scoreLabel->Initialize("SCORE:", m_x + m_xPad, m_y + 32 + m_yPad);
	UpdateScore();

	/****** High Score ******/
	m_highScoreLabel->Initialize("HIGH:", m_x + m_xPad, m_y + 32 + 32 + m_yPad);
	m_score->SetHighScore(0);

	// Retrieve the high score as an integer
	int highScoreValue = m_score->GetHighScore();

	// Ensure the score is limited to 7 digits
	if (highScoreValue > 9999999)
		highScoreValue = 9999999;

	std::string highScore = std::to_string(highScoreValue);
	highScore = std::string(7 - highScore.length(), '0') + highScore;

	m_highScoreAsText->Initialize(highScore, m_x + m_column2XOffset, m_y + 32 + 32 + m_yPad);
}

void GameEngine::HeadsUpDisplay::UpdateScore()
{
	int scoreValue = m_score->GetScore();

	// Ensure the score is limited to 7 digits
	if (scoreValue > 9999999)
		scoreValue = 9999999;

	// Convert the integer score to a string with leading zeros and a width of 7 characters
	std::string score = std::to_string(scoreValue);
	score = std::string(7 - score.length(), '0') + score;

	m_scoreAsText->Initialize(score, m_x + m_column2XOffset, m_y + +32 + m_yPad);

	// Set HighScore
	int highScoreValue = m_score->GetHighScore();
	// Ensure the score is limited to 7 digits
	if (highScoreValue > 9999999)
		highScoreValue = 9999999;

	std::string highScore = std::to_string(highScoreValue);
	highScore = std::string(7 - highScore.length(), '0') + highScore;

	m_highScoreAsText->Initialize(highScore, m_x + m_column2XOffset, m_y + 32 + 32 + m_yPad);
}

void GameEngine::HeadsUpDisplay::IncreaseScore(int amount)
{
	m_score->IncreaseScore(amount);
}

void GameEngine::HeadsUpDisplay::SetUpdateRequired(bool updateRequired)
{
	m_updatedRequired = updateRequired;
}

bool GameEngine::HeadsUpDisplay::UpdateRequired()
{
	return m_updatedRequired;
}

void GameEngine::HeadsUpDisplay::ResetScore()
{
	m_score->SetScore(0);
	UpdateScore();
	m_updatedRequired = true;
}

void HeadsUpDisplay::DrawHud()
{
	m_background->Draw();
	m_levelLabel->DrawText(1.0f);
	m_levelAsText->DrawText(1.0f);
	m_scoreLabel->DrawText(1.0f);
	m_scoreAsText->DrawText(1.0f);
	m_highScoreLabel->DrawText(1.0f);
	m_highScoreAsText->DrawText(1.0f);
}

