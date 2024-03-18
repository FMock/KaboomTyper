#pragma once
#include "ILevel.h"

namespace GameEngine
{
	class LevelGamePlay : public ILevel
	{
	public:
		void LoadAssets() override;
		void Update(float dt) override;
		void Render() override;
		void Enter() override;
		void Exit() override;
	};
}