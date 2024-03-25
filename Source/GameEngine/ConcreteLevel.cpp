#include "ConcreteLevel.h"

using namespace GameEngine;

ConcreteLevel::ConcreteLevel()
{   
     m_spriteFactoryPtr = std::make_unique<ConcreteSpriteFactory>();   
}

void ConcreteLevel::LoadAssets()
{
}

void ConcreteLevel::Update(float dt)
{
}

void ConcreteLevel::Render()
{
}

void ConcreteLevel::Enter()
{
}

void ConcreteLevel::Exit()
{
}
