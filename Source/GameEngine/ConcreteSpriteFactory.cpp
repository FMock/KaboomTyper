#include "ConcreteSpriteFactory.h"
#include "TextBlock.h"

using namespace GameEngine;

Sprite* ConcreteSpriteFactory::GetSprite(SpriteType type)
{
	switch (type)
	{
	case SpriteType::None:
		return nullptr;
	case SpriteType::TextBlock:
		return new TextBlock();
		// Add more cases for each sprite type...
	default:
		return nullptr; // If the sprite type is not recognized, return null or throw an exception.
	}
}