#pragma once

#include <string>


namespace GameEngine
{
    enum class GameState
    {
        IDLE,
        RUNNING,
        PAUSED,
        STOPPED,
        NUMBER_STATES // Keep this as the last enum value for easy counting
    };
}