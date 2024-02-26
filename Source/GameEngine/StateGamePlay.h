#pragma once
#include "IState.h"

namespace GameEngine
{
	class StateGamePlay : public IState
	{
	public:
		StateGamePlay() = default;
		void LoadAssets() override;
		void Update(float dt) override;
		void Render() override;
		void Enter() override;
		void Exit() override;
	};
}
