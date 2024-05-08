#pragma once

#include <iostream>
#include <functional>
#include <unordered_map>
#include "GameStates.h"

class StateMachine; // forward declaration for transition function definition

// Define transition function type for convenience
using TransitionFunction = std::function<void(StateMachine*)>;

class StateMachine 
{
public:
    StateMachine();
    void TransitionTo(GameEngine::GameState nextState);
    GameEngine::GameState GetCurrentState() const;
    std::string GetCurrentStateAsString() const;

private:

    // Define transition table type
    using TransitionTable = std::unordered_map<GameEngine::GameState, std::unordered_map<GameEngine::GameState, TransitionFunction>>;

    // Define transition functions
    void StartRunning();
    void Stop();
    void Pause();
    void Reset();

    // Initialize transition table
    static TransitionTable InitializeTransitionTable();

    GameEngine::GameState m_currentState;
    const TransitionTable m_transitionTable;
};
