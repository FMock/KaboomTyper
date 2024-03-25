#include "ConcreteSpriteFactory.h"
#include "TextBlock.h"

using namespace GameEngine;

std::unique_ptr<Sprite> ConcreteSpriteFactory::CreateSprite(SpriteType type)
{
	switch (type)
	{
	case SpriteType::TextBlock:
		return std::make_unique<TextBlock>();
		break;
	case SpriteType::Leaf:
		// Create and return Leaf sprite
		break;
	case SpriteType::Ballon:
		// Create and return Ballon sprite
		break;
	default:
		throw std::invalid_argument("Invalid SpriteType");
	}
}
