#include "HeadsUpDisplay.h"

using namespace GameEngine;

HeadsUpDisplay::HeadsUpDisplay() 
	: m_background(std::make_unique<RectangleDrawable>()),
	  m_playerLabel(std::make_unique<TextString>()),
	  m_levelAsText(std::make_unique<TextString>()),
	  m_scoreLabel(std::make_unique<TextString>()),
	  m_scoreAsText(std::make_unique<TextString>()),
	  m_highScoreLabel(std::make_unique<TextString>()),
	  m_highScoreAsText(std::make_unique<TextString>()),
	  m_score(std::make_shared<Score>()),
	  m_user(std::make_shared<User>()),
	  m_x(0), m_y(0), m_width(0), m_height(0), m_updatedRequired(false),
	  m_priority(0)
{
	m_user->SetName("Frank");
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

	/****** Player  ******/
	m_playerLabel->Initialize("PLAYER:", m_x + m_xPad, m_y + m_yPad);

	int levelValue = 1;

	// Ensure the user name is limited to 7 characters
	auto name = m_user->GetName().substr(0, 7);

	m_levelAsText->Initialize(name, m_x + m_column2XOffset, m_y + 5);

	/****** Score ******/
	m_scoreLabel->Initialize(" SCORE:", m_x + m_xPad, m_y + 32 + m_yPad);
	UpdateScore();

	/****** High Score ******/
	m_highScoreLabel->Initialize("  HIGH:", m_x + m_xPad, m_y + 32 + 32 + m_yPad);
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

std::shared_ptr<Score> GameEngine::HeadsUpDisplay::GetScore() const
{
	return m_score;
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

void GameEngine::HeadsUpDisplay::SetUserName(std::string& name)
{
	// Ensure the user name is limited to 7 characters
	m_user->SetName(name);
	auto nameTruncated = m_user->GetName().substr(0, 7);
	m_levelAsText->Initialize(nameTruncated, m_x + m_column2XOffset, m_y + 5);
}

void HeadsUpDisplay::DrawHud()
{
	m_background->Draw();
	m_playerLabel->DrawText(0.9f);
	m_levelAsText->DrawText(0.9f);
	m_scoreLabel->DrawText(0.9f);
	m_scoreAsText->DrawText(0.9f);
	m_highScoreLabel->DrawText(0.9f);
	m_highScoreAsText->DrawText(0.9f);
}

