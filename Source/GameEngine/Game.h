#pragma once
#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "InputManager.h"
#include "Sprite.h"
#include "TextString.h"
#include "TextBlock.h"
#include "FontParameters.h"
#include "TextBlockParameters.h"
#include "StateManager.h"
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

		// Add a friend declaration for StateMachine
		friend class StateMachine;

	private:

		TextString* m_textStr;
		TextBlock* m_textBlock;
		TextBlockParameters m_textBlockParameters;
		std::unique_ptr<InputManager> m_inputManager;
		SDL_Window* m_window;
		SDL_GLContext m_glcontext;
		std::map<std::string, GLuint> m_stringToColoredBlockTextureMap;
		TextStringFont* m_font;
		FontParameters m_fontParameters;
		std::unique_ptr<StateManager> m_stateManager;
		std::unique_ptr<GameManager> m_gameManager;

		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();
		void LoadData();
		void UnloadData();
		void InitializeStates();
		void CleanupStates();

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
