#include "Moveable.h"

using namespace GameEngine;

GameEngine::Moveable::Moveable() : m_changeX(0), m_changeY(0), m_speedX(0), m_speedY(0), m_isMoving(false)
{
}

void Moveable::MoveRight()
{
	m_changeX += m_speedX;
	m_changeY = 0;
}

void Moveable::MoveLeft()
{
	m_changeX += -m_speedX;
	m_changeY = 0;
}

void Moveable::MoveUp()
{
	m_changeX = 0;
	m_changeY -= m_speedY;
}

void Moveable::MoveDown()
{
	m_changeX = 0;
	m_changeY += m_speedY;
}

void Moveable::Stop()
{
	m_changeX = 0;
	m_changeY = 0;
}