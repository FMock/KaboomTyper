#pragma once
#include "IState.h"

namespace GameEngine
{
	class StateMainMenu : public IState
	{
	public:
		void LoadAssets() override;
		void Update(float dt) override;
		void Render() override;
		void Enter() override;
		void Exit() override;
	};
}