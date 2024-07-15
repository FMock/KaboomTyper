#include "Score.h"
#include <iostream> // For logging (optional)

using namespace GameEngine;

Score::Score() : m_score(0), m_highScore(0), m_multiplier(1), m_totalGamesPlayed(0)
{
}

void Score::IncreaseScore(int score)
{
    m_score += score;
    SetHighScore(m_score);
}

void Score::DecreaseScore(int score)
{
    // Ensure the score doesn't become negative
    if (m_score >= score)
    {
        m_score -= score;
    }
    else
    {
        // Optionally, you can log an error or handle it in another way
        std::cerr << "Error: DecreaseScore called with score larger than current score." << std::endl;
    }
}

void Score::SetScore(int score)
{
    m_score = score;
    SetHighScore(m_score);
}

int Score::GetScore() const
{
    return m_score;
}

void Score::SetHighScore(int score)
{
    if (score > m_highScore)
    {
        m_highScore = score;
    }
}

int Score::GetHighScore() const
{
    return m_highScore;
}

int GameEngine::Score::GetMultiplier() const
{
    return m_multiplier;
}

void Score::IncreaseMultiplier(int multiplier)
{
    m_multiplier += multiplier;
}

void Score::DecreaseMultiplier(int multiplier)
{
    m_multiplier -= multiplier;
}

void Score::SaveScoreToFile(const std::string& filename) const
{
    // TODO
}

void Score::LoadScoreFromFile(const std::string& filename)
{
    // TODO
}

void Score::TriggerScoreEvent(int score)
{
    // TODO
}

void Score::DisplayScoreAnimation() const
{
    // TODO
}

void Score::AddToScoreboard(int score)
{
    // TODO
}


std::deque<int>& Score::GetScoreboard()
{
    return m_scoreboard;
}

std::vector<int> Score::GetTopScores() const
{
    // Create a vector to store the top 10 scores
    std::vector<int> topScores;

    // If the scoreboard contains fewer than 10 scores, return the entire scoreboard
    if (m_scoreboard.size() <= 10)
    {
        topScores = std::vector<int>(m_scoreboard.begin(), m_scoreboard.end());
    }
    else
    {
        // Copy the top 10 scores from the scoreboard to the vector
        topScores = std::vector<int>(m_scoreboard.begin(), m_scoreboard.begin() + 10);
    }

    return topScores;
}

void Score::ClearScoreboard()
{
    // TODO:
}

double Score::GetAverageScorePerGame() const
{
    // TODO
    return 0.0;
}

int Score::GetTotalGamesPlayed() const
{
    // TODO
    return 0;
}

void Score::ShareScoreOnSocialMedia() const
{
    // TODO
}
