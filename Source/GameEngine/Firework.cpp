#include "Firework.h"
#include "Common.h"
#include <cmath>

using namespace GameEngine;

Firework::Firework(GLuint texture, int x, int y, int w, int h, int numParticles)
    : m_tex(texture), m_posX(x), m_posY(y), m_width(w), m_height(h), m_particleCount(numParticles), m_timer(0.0f), m_isActive(false)
{
    std::srand(std::time(0));
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


/// <summary>
/// GenerateParticles generates particels that appear as a firework explosion. This method is using polar coordinates at 
/// random angles and radii for each particle. The angle ensures the particles are distributed in a circle, and the radius 
/// determines how far they are from the center.
/// </summary>
void Firework::SetIsActive(bool isActive)
{
    m_isActive = isActive;
}

void GameEngine::Firework::SetColor(Colors color)
{
    auto colorPtr = std::make_unique<Color>();
    std::string colorString = Color::s_colorMap[color];
    m_tex = colorPtr->s_colorParameters.m_stringColorTextureColorMap[colorString];
}

void GameEngine::Firework::SetPosition(int x, int y)
{
    m_posX = x;
    m_posY = y;
}

void Firework::GenerateParticles()
{
    m_particles.clear();
    m_timer = 0.0f;

    for (int i = 0; i < m_particleCount; ++i)
    {
        Particle p;

        float angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * 3.1415926535897932; // random angle in radians
        float radius = static_cast<float>(std::rand()) / RAND_MAX * 100.0f; // random radius, adjusted to be twice as large

        p.x = m_posX + static_cast<int>(radius * cos(angle)); // x-coordinate based on polar coordinates
        p.y = m_posY + static_cast<int>(radius * sin(angle)); // y-coordinate based on polar coordinates
        p.scaleX = 1.0f + static_cast<float>(std::rand()) / (RAND_MAX / 2.0f); // random scale between 1 and 3
        p.scaleY = 1.0f + static_cast<float>(std::rand()) / (RAND_MAX / 2.0f); // random scale between 1 and 3
        m_particles.push_back(p);
    }
}
