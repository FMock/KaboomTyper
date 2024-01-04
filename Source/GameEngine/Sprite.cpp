#include "Sprite.h"
#include <iostream>

GameEngine::Sprite::Sprite() : InputObserver()
{
	m_move = { "Left arrow pressed",
			   "Right arrow pressed",
			   "Up arrow pressed",
			   "Down arrow pressed" };
}

void GameEngine::Sprite::Update()
{
	int move = static_cast<int>(m_moveDirection);
	if(m_moveDirection != GameEngine::MoveDirection::NONE)
		std::cout << m_move[move] << std::endl;
}

void GameEngine::Sprite::ProcessInput()
{

}

void GameEngine::Sprite::RespondToObserved(InputManager* InputMgr)
{

	if (!InputMgr->m_kbPrevState[SDL_SCANCODE_RIGHT] && InputMgr->m_kbState[SDL_SCANCODE_RIGHT])
	{
		m_moveDirection = GameEngine::MoveDirection::RIGHT;
	}
	else if (!InputMgr->m_kbPrevState[SDL_SCANCODE_LEFT] && InputMgr->m_kbState[SDL_SCANCODE_LEFT])
	{
		m_moveDirection = GameEngine::MoveDirection::LEFT;
	}
	else if (!InputMgr->m_kbPrevState[SDL_SCANCODE_UP] && InputMgr->m_kbState[SDL_SCANCODE_UP])
	{
		m_moveDirection = GameEngine::MoveDirection::UP;
	}
	else if (!InputMgr->m_kbPrevState[SDL_SCANCODE_DOWN] && InputMgr->m_kbState[SDL_SCANCODE_DOWN])
	{
		m_moveDirection = GameEngine::MoveDirection::DOWN;
	}
	else
	{
		m_moveDirection = GameEngine::MoveDirection::NONE;
	}
}
