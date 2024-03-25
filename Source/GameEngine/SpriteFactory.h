#pragma once
#include "Sprite.h"
#include "SpriteType.h"
#include <memory>

namespace GameEngine
{
	/// <summary>
	/// Pure abstract SpriteFactory class
	/// </summary>
	class SpriteFactory
	{
	public:
		virtual ~SpriteFactory() {} // Virtual destructor for polymorphism.
		virtual std::unique_ptr<Sprite> CreateSprite(SpriteType type) = 0; // Pure virtual function, making SpriteFactory an abstract class.
	};
}
