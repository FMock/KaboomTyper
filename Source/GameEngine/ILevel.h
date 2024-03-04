#pragma once
#include <memory>
#include "SpriteFactory.h"

namespace GameEngine
{ 
	/// <summary>
	/// Interface for all Levels'
	/// </summary>
	class ILevel
	{
	public:
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;

	protected:
		std::unique_ptr<SpriteFactory> m_spriteFactoryPtr;
	};
}