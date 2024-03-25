#pragma once

#include "GameStates.h"

// To use the GameState enum class as a key in a std::map, you need to provide a custom comparator
// since  an enum class does not implicitly convert to integral types, which std::map requires for
//  ordering keys.

namespace GameEngine
{
    // Custom comparator for GameState
    struct GameStateComparator
    {
        bool operator()(GameState lhs, GameState rhs) const
        {
            return static_cast<int>(lhs) < static_cast<int>(rhs);
        }
    };
}