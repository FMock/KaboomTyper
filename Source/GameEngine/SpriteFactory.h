#pragma once
#include "Sprite.h"
#include "SpriteType.h"

namespace GameEngine
{
	/// <summary>
	/// Pure abstract SpriteFactory class
	/// </summary>
	class SpriteFactory
	{
	public:
		virtual ~SpriteFactory() {} // Virtual destructor for polymorphism.
		virtual Sprite* GetSprite(SpriteType type) = 0; // Pure virtual function, making SpriteFactory an abstract class.
	};
}
