#pragma once
#include "Firework.h"
#include "IDrawable.h"
#include <memory>
#include <vector>

namespace GameEngine
{
	class FireworkExplosionManager : public IDrawable
	{
	public:
		FireworkExplosionManager();
		void Update(float dt) override;
		void Draw() override;
		int GetPriority() const override { return m_priority; }
		void SetPriority(int priority) override { m_priority = priority; }

		// Spawn explosions for the just-destroyed TextBlock (reads Common globals for its
		// position/width). Long blocks get several staggered blasts spread across the block.
		// Returns the number of blasts spawned (used to scale the audio).
		int Trigger();

	private:
		std::vector<GLuint> m_explosionFrames;                 // shared hit_* animation frames
		std::vector<std::unique_ptr<Firework>> m_explosions;   // active explosion pool
		int m_priority; // draw priority
	};
}
