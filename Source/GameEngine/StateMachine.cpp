#include "StateMachine.h"
#include "GlobalPreprocessorFlags.h"

using namespace GameEngine;


/*
*  Usage:
*  StateMachine stateMachine;
*  stateMachine.transitionTo(GameState::RUNNING);
*  stateMachine.transitionTo(GameState::PAUSED);
*  etc...
*/

StateMachine::StateMachine() : m_currentState(GameState::IDLE), m_transitionTable(InitializeTransitionTable()) 
{
}

// Initializes the transition table
StateMachine::TransitionTable StateMachine::InitializeTransitionTable() 
{
    TransitionTable table = {
        { GameState::IDLE, {
            { GameState::RUNNING, &StateMachine::StartRunning },
        }},
        { GameState::RUNNING, {
            { GameState::PAUSED, &StateMachine::Pause },
            { GameState::STOPPED, &StateMachine::Stop }
        }},
        { GameState::PAUSED, {
            { GameState::RUNNING, &StateMachine::StartRunning },
            { GameState::STOPPED, &StateMachine::Stop }
        }},
        { GameState::STOPPED, {
            { GameState::IDLE, &StateMachine::Reset }
        }}
    };
    return table;
}

// Transitions the state machine to the next state.
// If nextState == m_currentState, a state change will not occur
void StateMachine::TransitionTo(GameEngine::GameState nextState)
{
    auto transition = m_transitionTable.find(m_currentState);

    if(transition != m_transitionTable.end())
    {
        auto action = transition->second.find(nextState);

        if(action != transition->second.end())
        {
            action->second(this);
            m_currentState = nextState;
        }
    }
}

// Returns the current state of the StateMachine
GameEngine::GameState StateMachine::GetCurrentState() const
{
    return m_currentState;
}

// Returns the current state as a std::string
// Useful for logging or Debugging
std::string StateMachine::GetCurrentStateAsString() const
{
	switch (m_currentState)
    {
        case GameState::IDLE: return "IDLE";
        case GameState::RUNNING: return "RUNNING";
        case GameState::PAUSED: return "PAUSED";
        case GameState::STOPPED: return "STOPPED";
        default: return "UNKNOWN";
    }
}

// Transition Functions. TODO: CHANGE THESE TO DO ACTUAL WORK
void StateMachine::StartRunning()
{
#if DEBUG
    std::cout << "Starting to run" << std::endl; 
#endif
}

void StateMachine::Pause()
{
#if DEBUG
    std::cout << "Pause" << std::endl; 
#endif
}

void StateMachine::Stop()
{
#if DEBUG
    std::cout << "Stop" << std::endl; 
#endif
}

void StateMachine::Reset()
{
#if DEBUG
    std::cout << "Reset" << std::endl;
#endif
}