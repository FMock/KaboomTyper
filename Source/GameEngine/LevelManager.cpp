#include "LevelManager.h"

using namespace GameEngine;

// Adds a level to the manager.
void LevelManager::AddLevel(std::shared_ptr<ILevel> level)
{
    m_levels.push_back(std::move(level));
}

void LevelManager::RemoveLevel(const std::shared_ptr<ILevel>& level)
{
    auto it = std::find(m_levels.begin(), m_levels.end(), level);
    if (it != m_levels.end())
    {
        m_levels.erase(it);
    }
}

void LevelManager::ClearLevels()
{
    m_levels.clear();
}

size_t LevelManager::GetLevelCount() const
{
    return m_levels.size();
}

std::shared_ptr<ILevel> LevelManager::GetLevel(size_t index) const
{
    if (index < m_levels.size())
    {
        return m_levels[index];
    }
    return nullptr;
}

const std::vector<std::shared_ptr<ILevel>>& LevelManager::GetLevels() const
{
    return m_levels;
}