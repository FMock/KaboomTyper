#include "LevelSplashScreen.h"
#include "ConcreteSpriteFactory.h"
#include <memory>
#include <iostream>

using namespace GameEngine;

LevelSplashScreen::LevelSplashScreen()
{
	m_spriteFactoryPtr = std::make_unique<ConcreteSpriteFactory>();
}

void LevelSplashScreen::LoadAssets()
{
}

void LevelSplashScreen::Update(float dt)
{
}

void LevelSplashScreen::Render()
{

}

void LevelSplashScreen::Enter()
{
}

void LevelSplashScreen::Exit()
{
}
