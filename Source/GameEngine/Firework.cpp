#include "Firework.h"

using namespace GameEngine;

Firework::Firework(GLuint texture, int x, int y, int w, int h, int numParticles)
    : m_tex(texture), m_posX(x), m_posY(y), m_width(w), m_height(h), m_particleCount(numParticles), m_timer(0.0f), m_isActive(false)
{
    std::srand(std::time(0));
    GenerateParticles();
}

void GameEngine::Firework::Update(float deltaTime)
{
    if (!m_isActive)
        return;

    m_timer += deltaTime;

    if (m_timer >= 0.5f)
    { // Display particles for 0.5 seconds
        m_isActive = false;
    }
}

void GameEngine::Firework::Update()
{
}

void Firework::Draw()
{
    if (!m_isActive)
        return;

    for (auto& particle : m_particles) // this draw should occur over many cycles of the game loop
    {
        DrawUtilities::glDrawSpriteScaled(m_tex, particle.x, particle.y, m_width, m_height, particle.scaleX, particle.scaleY);
    }
}

void Firework::SetIsActive(bool isActive)
{
    m_isActive = isActive;
}

void Firework::GenerateParticles()
{
    m_particles.clear();
    m_timer = 0.0f;

    for (int i = 0; i < m_particleCount; ++i)
    {
        Particle p;
        p.x = m_posX + (std::rand() % 100 - 50); // random offset
        p.y = m_posY + (std::rand() % 100 - 50); // random offset
        p.scaleX = 1.0f + static_cast<float>(std::rand()) / RAND_MAX * 2.0f; // random scale between 0 and 2
        p.scaleY = 1.0f + static_cast<float>(std::rand()) / RAND_MAX * 2.0f; // random scale between 0 and 2
        m_particles.push_back(p);
    }
}