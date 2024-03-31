#include "Sprite.h"
#include <iostream>
#include "DrawUtils.h"

using namespace GameEngine;

GameEngine::Sprite::Sprite() : m_size(0, 0), m_position(0.0f, 0.0f), InputObserver()
{
	m_move = { "Left arrow pressed",
			   "Right arrow pressed",
			   "Up arrow pressed",
			   "Down arrow pressed" };
}

GameEngine::Sprite::Sprite(float x, float y, int w, int h) : m_size(w, h), m_position(x, y), InputObserver()
{
	m_image = NULL;
	m_box = AABB();
	m_box.setX(x);
	m_box.setY(y);
	m_box.setW(w);
	m_box.setH(h);
	setXPos(x);
	setYPos(y);
	m_change_x = 0;
	m_change_y = 0;
}

GameEngine::Sprite::~Sprite()
{
}

void Sprite::Initialize(float x, float y, int w, int h)
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
	setXPos(x);
	setYPos(y);
	m_change_x = 0;
	m_change_y = 0;;
}

void GameEngine::Sprite::Update(float dt)
{
}

void GameEngine::Sprite::Draw()
{
	DrawUtilities::glDrawSprite(m_image, m_position.first, m_position.second, m_size.first, m_size.second);
}

void GameEngine::Sprite::ProcessInput()
{
}

void GameEngine::Sprite::RespondToObserved(InputManager* InputMgr)
{
	// Do nothing. Let derived classes decide how to respond.
}


void GameEngine::Sprite::setXPos(float x)
{
	m_position.first = x;
	m_box.setX(abs(x));
}

float GameEngine::Sprite::getXPos() const
{
	return abs(m_position.first);
}

void GameEngine::Sprite::setYPos(float y)
{
	m_position.second = y;
	m_box.setY(abs(y));
}

float GameEngine::Sprite::getYPos() const
{
	return m_position.second;
}

std::pair<int, int> GameEngine::Sprite::getSize() const
{
	return m_size;
}

std::pair<float, float> GameEngine::Sprite::getPosition() const
{
	return m_position;
}

void GameEngine::Sprite::setWidth(int w)
{
	m_size.first = w;
	m_box.setW(w);
}

int GameEngine::Sprite::getWidth() const
{
	return m_size.first;
}

void GameEngine::Sprite::setHeight(int h)
{
	m_size.second = h;
	m_box.setH(h);
}

int GameEngine::Sprite::getHeight() const
{
	return m_size.second;
}
