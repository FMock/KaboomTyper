#include "StateManager.h"

using namespace GameEngine;



StateManager::StateManager() : m_currentState(GameState::SplashScreen), m_states{}
{

}

void StateManager::AddState(GameState state, IState* stateObject)
{
    if (stateObject != nullptr)
    { 
        m_states[state] = stateObject;
    }
    else
    {
        // Handle the error case where stateObject is null
    }
}

IState* GameEngine::StateManager::GetState(GameState state)
{
    if (!m_states.empty())
    {
        return m_states[state];
    }
}

void StateManager::SwitchState(GameState state)
{
    m_currentState = state;
    m_states[m_currentState]->Enter(); // Trigger enter event of the new state
}

void StateManager::Update(float dt)
{
    m_states[m_currentState]->Update(dt);
}

void StateManager::Render()
{
    m_states[m_currentState]->Render();
}

void StateManager::Exit()
{
    m_states[m_currentState]->Exit(); // Trigger exit event of the current state
}