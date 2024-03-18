#pragma once
#include "InputObserver.h"
#include "GameStates.h"
#include "LevelManager.h"
#include <vector>
#include <map>

namespace GameEngine 
{
	class GameManager : public InputObserver
	{
	public:
		void Update(float dt);
		void Render();

	private:
		GameState m_gameState;
		std::map<GameState, LevelManager> m_levels;
		void Initialize();

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}


