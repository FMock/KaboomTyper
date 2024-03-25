#include "Sprite.h"
#include <iostream>
#include "DrawUtils.h"

using namespace GameEngine;

GameEngine::Sprite::Sprite() : InputObserver()
{
	m_move = { "Left arrow pressed",
			   "Right arrow pressed",
			   "Up arrow pressed",
			   "Down arrow pressed" };

	m_characterSize = new int[2]();
	m_characterPos = new float[2]();
}

GameEngine::Sprite::Sprite(int x, int y, int w, int h) : InputObserver()
{
	m_image = NULL;
	m_characterSize = new int[2]();
	m_characterPos = new float[2]();
	m_characterSize[0] = w; // width
	m_characterSize[1] = h; // height
	box = AABB();
	box.setX(x);
	box.setY(y);
	box.setW(w);
	box.setH(h);
	setXPos(x);
	setYPos(y);
	change_x = 0;
	change_y = 0;
	m_type = new char[20];
	m_type = "sprite";
}

GameEngine::Sprite::~Sprite()
{
	delete[] m_characterSize;
	delete[] m_characterPos;
}

void Sprite::Initialize(int x, int y, int w, int h)
{
	m_image = NULL;
	m_characterSize = new int[2]();
	m_characterPos = new float[2]();
	m_characterSize[0] = w; // width
	m_characterSize[1] = h; // height
	box = AABB();
	box.setX(x);
	box.setY(y);
	box.setW(w);
	box.setH(h);
	setXPos(x);
	setYPos(y);
	change_x = 0;
	change_y = 0;
	m_type = new char[20];
	m_type = "sprite";
}

//void GameEngine::Sprite::Update()
//{
//	//int move = static_cast<int>(m_moveDirection);
//	//if(m_moveDirection != GameEngine::MoveDirection::NONE)
//	//	std::cout << m_move[move] << std::endl;
//}

void GameEngine::Sprite::Draw()
{
	DrawUtilities::glDrawSprite(m_image, m_characterPos[0], m_characterPos[1], m_characterSize[0], m_characterSize[1]);
}

void GameEngine::Sprite::ProcessInput()
{

}

void GameEngine::Sprite::RespondToObserved(InputManager* InputMgr)
{
	// Do nothing. Let derived classes decide how to respond.
}


void GameEngine::Sprite::setXPos(int x)
{
	m_characterPos[0] = x;
	box.setX(abs(m_characterPos[0]));
}

int GameEngine::Sprite::getXPos() const
{
	int x = abs(m_characterPos[0]);
	return x;
}

void GameEngine::Sprite::setYPos(int y)
{
	m_characterPos[1] = y;
	box.setY(abs(m_characterPos[1]));
}

int GameEngine::Sprite::getYPos() const
{
	return abs(m_characterPos[1]);
}

void GameEngine::Sprite::setWidth(int w)
{
	m_characterSize[0] = w;
	box.setW(w);
}

int GameEngine::Sprite::getWidth() const
{
	return m_characterSize[0];
}

void GameEngine::Sprite::setHeight(int h)
{
	m_characterSize[1] = h;
	box.setH(h);
}

int GameEngine::Sprite::getHeight() const
{
	return m_characterSize[1];
}
