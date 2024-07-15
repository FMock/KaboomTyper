#pragma once
#include "Firework.h"
#include "IDrawable.h"
#include <memory>

namespace GameEngine
{
	class FireworkExplosionManager : public IDrawable
	{
	public:
		FireworkExplosionManager();
		void ProcessInput();
		void Update(float dt) override;
		void Draw() override;
		int GetPriority() const override { return m_priority; }
		void SetPriority(int priority) override { m_priority = priority; }
		bool IsFireworkActive();
		void SetIsFireworkActive(bool isActive);
		void GenerateFireworkParticles();
		void SetShouldFireworkExplode(bool shouldExplode);
		bool GetShouldFireworkExplode();

	private:
		GLuint m_fireworkColorTexture;
		std::unique_ptr<Firework> m_firework;
		bool m_explode;
		int m_priority; // draw priority
	};
}
