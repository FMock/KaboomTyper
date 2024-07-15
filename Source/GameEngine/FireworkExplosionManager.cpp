#include "FireworkExplosionManager.h"
#include "Common.h"

GameEngine::FireworkExplosionManager::FireworkExplosionManager() : m_explode(false), m_priority(0)
{
    m_colorPtr = std::make_unique<Color>();
    m_fireworkColorTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap["green"];
    m_firework = std::make_unique<Firework>(m_fireworkColorTexture, 400, 400, 1, 1, 300);
}

void GameEngine::FireworkExplosionManager::ProcessInput()
{
    // Set Firework color and positon
    m_firework->SetColor(Common::s_previousColor);
    int x = (int)Common::s_currentPosition.first;
    int y = (int)Common::s_currentPosition.second;
    int adjustedX = x + (Common::s_currentTextBlockWidth / 2);
    int halfFontHeight = 16;
    int adjustedY = y + halfFontHeight;
    m_firework->SetPosition(adjustedX, adjustedY);
}

void GameEngine::FireworkExplosionManager::Update(float dt)
{
    m_firework->Update(dt);
}

void GameEngine::FireworkExplosionManager::Draw()
{
    if (m_firework->IsActive())
    {
        m_firework->Draw();
    }
}

bool GameEngine::FireworkExplosionManager::IsFireworkActive()
{
    return m_firework->IsActive();
}

void GameEngine::FireworkExplosionManager::SetIsFireworkActive(bool isActive)
{
    m_firework->SetIsActive(isActive);
}

void GameEngine::FireworkExplosionManager::GenerateFireworkParticles()
{
    m_firework->GenerateParticles();
}

void GameEngine::FireworkExplosionManager::SetShouldFireworkExplode(bool shouldExplode)
{
    m_explode = shouldExplode;
}

bool GameEngine::FireworkExplosionManager::GetShouldFireworkExplode()
{
    return m_explode;
}
