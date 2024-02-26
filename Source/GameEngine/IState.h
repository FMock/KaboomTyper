#pragma once

// Represents a game state. 

namespace GameEngine
{
	class IState
	{
	public:
		virtual void LoadAssets() = 0;
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;
		virtual void Enter() = 0;
		virtual void Exit() = 0;
	};
}