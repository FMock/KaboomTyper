#include "Firework.h"
#include "Common.h"
#include <cmath>
#include <memory>
#include "Color.h"

using namespace GameEngine;

Firework::Firework(int numParticles)
    : m_particleCount(numParticles)
{
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Bright spark colors so the blast is always visible on the dark-blue play area.
    m_texWhite = Color::s_colorParameters.whiteTexture;
    m_texYellow = Color::s_colorParameters.yellowTexture;
    m_texOrange = Color::s_colorParameters.m_stringColorTextureColorMap["orange"];
    if (m_texOrange == 0)
        m_texOrange = m_texYellow; // fall back if no orange texture
}

void Firework::Init(const std::vector<GLuint>* frames, int centerX, int centerY, int spriteSize, float startDelay)
{
    m_frames = frames;
    m_posX = centerX;
    m_posY = centerY;
    m_spriteSize = spriteSize;
    m_startDelay = startDelay;
    m_elapsed = 0.0f;
    m_frameIndex = 0;
    m_isActive = true;
    GenerateParticles();
}

void Firework::SetIsActive(bool isActive)
{
    m_isActive = isActive;
}

void Firework::GenerateSet(std::vector<Particle>& set, int count, float maxSpeed)
{
    set.clear();
    set.reserve(count);
    for (int i = 0; i < count; ++i)
    {
        Particle p;
        float angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * PI;
        float speed = (0.2f + 0.8f * static_cast<float>(std::rand()) / RAND_MAX) * maxSpeed;
        p.x = static_cast<float>(m_posX);
        p.y = static_cast<float>(m_posY);
        p.vx = std::cos(angle) * speed;
        p.vy = std::sin(angle) * speed;
        p.scale = 1.0f + 2.0f * static_cast<float>(std::rand()) / RAND_MAX; // small bright sparks/embers
        set.push_back(p);
    }
}

void Firework::GenerateParticles()
{
    m_elapsed = 0.0f;
    GenerateSet(m_sparksWhite, m_particleCount, 340.0f);
    GenerateSet(m_sparksYellow, m_particleCount / 2, 270.0f);
    GenerateSet(m_sparksOrange, m_particleCount / 2, 220.0f);
}

void GameEngine::Firework::Update(float deltaTime)
{
    if (!m_isActive)
        return;

    m_elapsed += deltaTime;

    if (m_elapsed < m_startDelay)
        return; // staggered blast hasn't started yet

    float animTime = m_elapsed - m_startDelay;
    m_frameIndex = static_cast<int>(animTime * m_animFps);

    if (m_frames && m_frameIndex >= static_cast<int>(m_frames->size()))
    {
        m_isActive = false;
        return;
    }

    // Fling the sparks outward with a little gravity.
    auto move = [deltaTime](std::vector<Particle>& set)
    {
        for (auto& p : set)
        {
            p.x += p.vx * deltaTime;
            p.y += p.vy * deltaTime;
            p.vy += 300.0f * deltaTime; // gravity
        }
    };
    move(m_sparksWhite);
    move(m_sparksYellow);
    move(m_sparksOrange);
}

void Firework::Draw()
{
    if (!m_isActive || m_elapsed < m_startDelay)
        return;

    // Bright additive sparks (drawn first, fireball over them). Small so they read as
    // embers, not blocks: effective size = sparkBase * scale = ~2..6 px.
    const int sparkBase = 2;
    for (auto& p : m_sparksOrange)
        DrawUtilities::glDrawSpriteWithGlow(m_texOrange, static_cast<int>(p.x), static_cast<int>(p.y), sparkBase, sparkBase, p.scale, p.scale);
    for (auto& p : m_sparksYellow)
        DrawUtilities::glDrawSpriteWithGlow(m_texYellow, static_cast<int>(p.x), static_cast<int>(p.y), sparkBase, sparkBase, p.scale, p.scale);
    for (auto& p : m_sparksWhite)
        DrawUtilities::glDrawSpriteWithGlow(m_texWhite, static_cast<int>(p.x), static_cast<int>(p.y), sparkBase, sparkBase, p.scale, p.scale);

    // Sprite fireball frame, centered on the explosion point.
    if (m_frames && m_frameIndex < static_cast<int>(m_frames->size()))
    {
        GLuint frame = (*m_frames)[m_frameIndex];
        DrawUtilities::glDrawSprite(frame, m_posX - m_spriteSize / 2, m_posY - m_spriteSize / 2, m_spriteSize, m_spriteSize);
    }
}
