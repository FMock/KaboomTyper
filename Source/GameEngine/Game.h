#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include <memory>
#include "GameManager.h"


namespace GameEngine
{
	class Game
	{
	public:
		Game();
		bool Initialize();
		void RunLoop();
		void Shutdown();

	private:
		SDL_Window* m_window;
		SDL_GLContext m_glcontext;
		
		void ProcessInput();
		void Update();
		void Draw();
		void LoadData();

		// Manages child game managers
		std::shared_ptr<GameManager> m_gameManager;

		// Set this to true to exit the game loop
		char m_shouldExit = 0;

		// To regulate frame rate
		unsigned int m_previousTime = 0;
		unsigned int m_currentTime = 0;
		unsigned int m_f_previousTime = 0;
		unsigned int m_fps = 0;
		double m_deltaTime = 0.0f;
	};
}
