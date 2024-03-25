#pragma once
#include "ILevel.h"

namespace GameEngine
{
	class LevelSplashScreen : public ILevel 
	{
	public:
		LevelSplashScreen();
		void LoadAssets() override;
		void Update(float dt) override;
		void Render() override;
		void Enter() override;
		void Exit() override;
	};
}