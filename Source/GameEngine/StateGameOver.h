#pragma once
#include "IState.h"

namespace GameEngine
{
	class StateGameOver : public IState
	{
	public:
		StateGameOver() = default;
		void LoadAssets() override;
		void Update(float dt) override;
		void Render() override;
		void Enter() override;
		void Exit() override;
	};
}