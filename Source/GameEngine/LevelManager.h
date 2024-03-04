#pragma once

#include <vector>
#include <memory>
#include "ILevel.h"

namespace GameEngine
{
    class LevelManager
    {
    public:
        LevelManager() = default; // default constructor
        ~LevelManager() = default; // default destructor

        // Adds a level to the manager.
        void AddLevel(std::shared_ptr<ILevel> level);

        // Removes a level from the manager.
        void RemoveLevel(const std::shared_ptr<ILevel>& level);

        // Removes all levels from the manager.
        void ClearLevels();

        // Returns the number of levels in the manager.
        size_t GetLevelCount() const;

        // Returns the level at the specified index.
        std::shared_ptr<ILevel> GetLevel(size_t index) const;

        // Accessor method to retrieve the list of levels.
        const std::vector<std::shared_ptr<ILevel>>& GetLevels() const;

    private:
        std::vector<std::shared_ptr<ILevel>> m_levels;
    };
}
