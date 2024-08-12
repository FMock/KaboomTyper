#include "Instructions.h"

using namespace GameEngine;

Instructions::Instructions() : m_priority(0), m_isActive(false)
{
	m_x = X_POSITION;
	m_y = Y_POSITION;
	
	m_menuBody = std::make_unique<RectangleDrawable>();
	m_menuBody->Initialize(m_x, m_y, WIDTH, HEIGHT, Colors::BLUE, true);
}

void GameEngine::Instructions::Draw()
{
	if (GetIsActive())
	{
		m_menuBody->Draw();
	}
}

void GameEngine::Instructions::Update(float dt)
{
}

bool GameEngine::Instructions::GetIsActive() const
{
	return m_isActive;
}

void GameEngine::Instructions::SetIsActive(bool isActive)
{
	m_isActive = isActive;
}

void GameEngine::Instructions::RespondToObserved(InputManager* InputMgr)
{
}
