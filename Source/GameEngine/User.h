#pragma once

#include <string>

namespace GameEngine
{
    class User
    {
    public:
        // Constructor
        User(const std::string& name, int health, int score, float x, float y);
        User();

        // Getters
        const std::string& GetName() const;
        int GetHealth() const;
        int GetScore() const;
        float GetX() const;
        float GetY() const;

        // Setters
        void SetName(const std::string& name);
        void SetHealth(int health);
        void SetScore(int score);
        void SetPosition(float x, float y);

        // Methods
        void Move(float deltaX, float deltaY);
        void TakeDamage(int amount);
        void AddScore(int points);

    private:
        std::string m_name;
        int m_health;
        int m_score;
        float m_x;
        float m_y;
    };
}
