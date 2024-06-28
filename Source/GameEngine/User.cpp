#include "User.h"

namespace GameEngine
{
    User::User(const std::string& name, int health, int score, float x, float y)
        : m_name(name), m_health(health), m_score(score), m_x(x), m_y(y)
    {
    }

    User::User() : User("", 0, 0, 0.0f, 0.0f)
    {
    }

    const std::string& User::GetName() const
    {
        return m_name;
    }

    int User::GetHealth() const
    {
        return m_health;
    }

    int User::GetScore() const
    {
        return m_score;
    }

    float User::GetX() const
    {
        return m_x;
    }

    float User::GetY() const
    {
        return m_y;
    }

    void User::SetName(const std::string& name)
    {
        m_name = name;
    }

    void User::SetHealth(int health)
    {
        m_health = health;
    }

    void User::SetScore(int score)
    {
        m_score = score;
    }

    void User::SetPosition(float x, float y)
    {
        m_x = x;
        m_y = y;
    }

    void User::Move(float deltaX, float deltaY)
    {
        m_x += deltaX;
        m_y += deltaY;
    }

    void User::TakeDamage(int amount)
    {
        m_health -= amount;
        if (m_health < 0)
        {
            m_health = 0;
        }
    }

    void User::AddScore(int points)
    {
        m_score += points;
    }
}
