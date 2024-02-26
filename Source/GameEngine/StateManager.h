#pragma once
#include <map>
#include "IState.h"
//#include "StateMachine.h"
#include "GameStates.h"

// Manages different game states

namespace GameEngine
{
    class StateManager
    {
    public:
        StateManager();
        void AddState(GameState state, IState* stateObject);
        IState* GetState(GameState state);
        void SwitchState(GameState state);
        void Update(float dt);
        void Render();
        void Exit();

        // Other methods for state management

    private:
        GameState m_currentState;
        std::map<GameState, IState*> m_states;
    };
}