#include "SideBar.h"


void GameEngine::SideBar::Initialize(int x, int y, int width, int height, Colors rectColor, bool filled)
{
	RectangleDrawable::Initialize(x, y, width, height, rectColor, filled);
}

void GameEngine::SideBar::Update(float dt)
{
	RectangleDrawable::Update(dt);
}

void GameEngine::SideBar::Draw()
{
	RectangleDrawable::Draw();
}
