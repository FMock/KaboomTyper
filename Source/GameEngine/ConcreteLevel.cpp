#include "ConcreteLevel.h"

using namespace GameEngine;

ConcreteLevel::ConcreteLevel()
{   
     m_spriteFactoryPtr = std::make_unique<ConcreteSpriteFactory>();   
}
