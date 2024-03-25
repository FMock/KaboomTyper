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
		virtual void LoadAssets() = 0;
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;
		virtual void Enter() = 0;
		virtual void Exit() = 0;

		// Getter for m_spriteFactoryPtr
		virtual std::unique_ptr<SpriteFactory>& GetSpriteFactory()
		{
			return m_spriteFactoryPtr;
		}

	protected:
		std::unique_ptr<SpriteFactory> m_spriteFactoryPtr;
	};
}