#pragma once
#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "InputManager.h"
#include "Sprite.h"
#include "TextString.h"

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
		Sprite* m_sprite;
		TextString* m_textStr;
		InputManager* m_InputManager;
		SDL_Window* m_window;
		SDL_GLContext m_glcontext;
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();
		void LoadData();
		void UnloadData();

		// The previous frame's keyboard state.
		unsigned char m_kbPrevState[SDL_NUM_SCANCODES] = { 0 };

		// The current frame's keyboard state.
		const unsigned char* m_kbState = NULL;

		// Set this to true to make the game loop exit.
		char m_shouldExit = 0;

		// To regulate frame rate
		unsigned int m_previousTime = 0;
		unsigned int m_currentTime = 0;
		unsigned int m_f_previousTime = 0;
		unsigned int m_f_currentTime = 0;
		float m_deltaTime = 0.0f;
		float m_noKeyPressTime = 0.0f;
		float m_msPerFrame = 0.0f;
		unsigned int m_seconds = 0;
		unsigned int m_fps = 0;
	};
}
