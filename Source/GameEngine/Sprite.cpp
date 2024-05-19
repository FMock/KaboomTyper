#include "Sprite.h"
#include <iostream>
#include "DrawUtils.h"

using namespace GameEngine;


GameEngine::Sprite::Sprite(float x, float y, float w, float h) 
	: m_size(w, h), m_position(x, y), m_moveDirection(MoveDirection::NONE), m_moveable(std::make_unique<Moveable>())
{
	m_image = NULL;
	m_box = AABB();
	m_box.setX(x);
	m_box.setY(y);
	m_box.setW(w);
	m_box.setH(h);
}

void Sprite::Initialize(float x, float y, float w, float h)
{
	m_image = NULL;
	m_size.first = w;
	m_size.second = h;
	m_position.first = x;
	m_position.second = y;
	m_box = AABB();
	m_box.setX(x);
	m_box.setY(y);
	m_box.setW(w);
	m_box.setH(h);
}

std::pair<float, float> GameEngine::Sprite::GetSize() const
{
	return m_size;
}

void GameEngine::Sprite::SetPosition(float x, float y)
{
	m_position.first = x;
	m_position.second = y;
}

void GameEngine::Sprite::SetSize(float width, float height)
{
	if (width >= 1)
		m_size.first = width;

	if (height >= 1)
		m_size.second = height;
}

std::pair<float, float> GameEngine::Sprite::GetPosition() const
{
	return m_position;
}

AABB& GameEngine::Sprite::GetBox()
{
	return m_box;
}