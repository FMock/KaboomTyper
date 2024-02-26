#pragma once

#include "IState.h"

namespace GameEngine
{
	class StateSplashScreen : public IState
	{
	public:
		StateSplashScreen() = default;
		void LoadAssets() override;
		void Update(float dt) override;
		void Render() override;
		void Enter() override;
		void Exit() override;

	private:
		
	};
}