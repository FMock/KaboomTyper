#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

namespace GameEngine
{
    class Score
    {
    public:
        Score();
        ~Score();

        void IncreaseScore(int score);
        void DecreaseScore(int score);
        void SetScore(int score);
        int GetScore() const;
        void SetHighScore(int score);
        int GetHighScore() const;

        int GetMultiplier() const;
        void IncreaseMultiplier(int multiplier);
        void DecreaseMultiplier(int multiplier);

        void SaveScoreToFile(const std::string& filename) const;
        void LoadScoreFromFile(const std::string& filename);

        void TriggerScoreEvent(int score); // Example: Triggers event/achievement based on score
        void DisplayScoreAnimation() const; // Example: Display animation when score changes

        void AddToScoreboard(int score); // Example: Add score to a high score list
        std::vector<int> GetTopScores() const;
        void ClearScoreboard(); // Example: Clear high score list

        std::deque<int>& GetScoreboard(); // Getter for the scoreboard

        double GetAverageScorePerGame() const; // Example: Calculate average score per game
        int GetTotalGamesPlayed() const; // Example: Get total number of games played

        void ShareScoreOnSocialMedia() const; // Example: Share score on social media platforms

    private:
        int m_score;
        int m_highScore;
        int m_multiplier;
        int m_totalGamesPlayed;
        std::deque<int> m_scoreboard; // Holds 100 most current scores
    };
}
