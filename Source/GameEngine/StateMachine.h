#pragma once

#include <iostream>
#include <memory>
#include "GameStates.h"

namespace GameEngine
{
    class StateMachine
    {
    public:

        StateMachine() = default;
        void SetCurrentState(GameState newState);
        inline GameState GetCurrentState() const { return m_currentState; }
        void Update();
        void PromptPlayAgainOrQuit();

    private:

        GameState m_currentState;
        GameState m_previousState;

    };
}
