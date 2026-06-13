#include "FireworkExplosionManager.h"
#include "Common.h"
#include "DrawUtils.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>

using namespace GameEngine;

FireworkExplosionManager::FireworkExplosionManager() : m_priority(0)
{
    // Load the 12-frame explosion animation once (reused by every blast).
    for (int i = 0; i < 12; ++i)
    {
        char path[128];
        std::snprintf(path, sizeof(path), "../../Resources/images/hit_%02d.tga", i);
        GLuint tex = DrawUtilities::glTexImageTGAFile(path);
        if (tex != 0)
            m_explosionFrames.push_back(tex);
    }
}

int FireworkExplosionManager::Trigger()
{
    if (m_explosionFrames.empty())
        return 0;

    int x = static_cast<int>(Common::s_currentPosition.first);
    int y = static_cast<int>(Common::s_currentPosition.second);
    int width = Common::s_currentTextBlockWidth;
    int centerY = y + 16; // half font height

    // One blast per ~64px of block width, so longer words erupt with more explosions.
    int blasts = std::max(1, std::min(6, width / 64));

    for (int i = 0; i < blasts; ++i)
    {
        // Spread the blasts across the block width with a little jitter.
        int cx = (blasts == 1)
            ? x + width / 2
            : x + static_cast<int>((static_cast<float>(i) + 0.5f) / blasts * width);
        cx += (std::rand() % 21) - 10;                 // +/-10 px x jitter
        int cy = centerY + (std::rand() % 21) - 10;    // +/-10 px y jitter

        int size = 100 + (std::rand() % 50);           // 100..149 px fireball
        float startDelay = (i == 0) ? 0.0f : (std::rand() % 12) / 100.0f; // 0..0.11s stagger

        auto firework = std::make_unique<Firework>(120);
        firework->Init(&m_explosionFrames, cx, cy, size, startDelay);
        m_explosions.push_back(std::move(firework));
    }

    return blasts;
}

void GameEngine::FireworkExplosionManager::Update(float dt)
{
    for (auto& explosion : m_explosions)
        explosion->Update(dt);

    // Remove finished explosions.
    m_explosions.erase(
        std::remove_if(m_explosions.begin(), m_explosions.end(),
            [](const std::unique_ptr<Firework>& e) { return !e->IsActive(); }),
        m_explosions.end());
}

void GameEngine::FireworkExplosionManager::Draw()
{
    for (auto& explosion : m_explosions)
        explosion->Draw();
}
