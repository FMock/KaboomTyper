#pragma once

#include "Game.h"
#include <GL/glew.h>
#include <time.h>
#include "Sprite.h"
#include "TextString.h"
#include "TextStringFont.h"
#include "TextBlock.h"
#include "DrawUtils.h"
#include "Utilities.h"
#include "StateMachine.h"
#include "StateMainMenu.h"
#include "StateSplashScreen.h"
#include "StateGamePlay.h"
#include "StateGameOver.h"

using namespace GameEngine;
using namespace GameEngine::Utility;

Game::Game()
{
	// empty constructor to ensure that members are created after the OpenGL context is set up
}
	
	

/// <summary>
/// Creates an SDL2 game windows with an OpenGL context
/// </summary>
/// <returns></returns>
bool Game::Initialize()
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Could not initialize SDL. ErrorCode=%s\n", SDL_GetError());
		return false;
	}

	// Create the window and OpenGL context.
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window = SDL_CreateWindow(
	"Kaboom Typer",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	800, 600,
	SDL_WINDOW_OPENGL);

	if (!m_window)
	{
		fprintf(stderr, "Could not create window. ErrorCode=%s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(m_window);

	// Make sure we have a recent version of OpenGL.
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		fprintf(stderr, "Could not initialize glew. ErrorCode=%s\n", glewGetErrorString(glewError));
		SDL_Quit();
		return 1;
	}
	if (GLEW_VERSION_2_0)
	{
		fprintf(stderr, "OpenGL 2.0 or greater supported: Version = %s\n",
			glGetString(GL_VERSION));
	}
	else
	{
		fprintf(stderr, "OpenGL max supported version is too low.\n");
		SDL_Quit();
		return 1;
	}

	// Setup OpenGL state.
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 800, 600, 0, 0, 100);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	srand(time(NULL));

	// Load the game characters and other graphics
	LoadData();

	return true;
}

/// <summary>
/// Load game assets and objects
/// </summary>
void Game::LoadData()
{
	int x = 50; // x position to draw on screen
	int y = 50; // y position to draw on screen

	m_textStr = new TextString();
	m_textStr->Initialize("Press Spacebar to Begin", x, y); // draw some text to the screen

	// Create a TextBlock at a radom position in the window
	srand(time(0));
	int randomX = rand() % 500;
	int randomY = rand() % 500;
	m_textBlock = new TextBlock(randomX, randomY, std::string("Kaboom Typer!"));

	// Load game states
	m_stateManager = std::make_unique<StateManager>();
	InitializeStates();
	m_stateManager->SwitchState(GameState::SPLASH_SCREEN);

	// Initialize Input Manager
	m_inputManager = std::make_unique<InputManager>();
	m_inputManager->RegisterObserver(m_textBlock);

	// Load GameManager
	m_gameManager = std::make_unique<GameManager>();
}

void Game::RunLoop()
{
	while (!m_shouldExit)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	//m_inputManager->Update();
	m_shouldExit = m_inputManager->ShouldQuit();
}

void Game::UpdateGame()
{
	// Compute deltaTime - the time difference between each frame
	m_currentTime = SDL_GetTicks();
	m_msPerFrame = m_currentTime - m_previousTime; // ~14 ms
	m_deltaTime = (m_msPerFrame) / 1000.0f; // ~ 0.014
	m_previousTime = m_currentTime;

	// Calculate FPS and print
	m_f_currentTime = SDL_GetTicks();
	if (m_f_currentTime > m_f_previousTime + 1000)
	{
		m_seconds++;
		printf("FPS: %i\n", m_fps);
		m_fps = 0;
		m_f_previousTime = m_f_currentTime;
	}

	m_fps++; // increment frame counter each iteration

	m_stateManager->Update(m_deltaTime);
	m_inputManager->Update();
	m_textBlock->Update(m_deltaTime);
	m_textStr->Update(m_deltaTime);
	//fmod_sys->update(); // If you don't update the sound will play once

	m_gameManager->Update(m_deltaTime);
}

void Game::GenerateOutput()
{
	// Draw Frame
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this

	// Draw Objects
	m_stateManager->Render();
	m_textBlock->Draw();
	m_textStr->DrawText();

	m_gameManager->Render();

	// Swap Window
	SDL_GL_SwapWindow(m_window);
}

void Game::Shutdown()
{
	UnloadData();
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(m_glcontext);
	SDL_Quit();
}

void Game::UnloadData()
{
	delete m_textStr;
	delete m_textBlock;
	CleanupStates();
}

void Game::InitializeStates()
{
	m_stateManager->AddState(GameState::SPLASH_SCREEN, new StateSplashScreen());
	m_stateManager->AddState(GameState::MAIN_MENU, new StateMainMenu());
	m_stateManager->AddState(GameState::GAME_PLAY, new StateGamePlay());
	m_stateManager->AddState(GameState::GAME_OVER, new StateGameOver());
}

void Game::CleanupStates()
{
	delete m_stateManager->GetState(GameState::SPLASH_SCREEN);
	delete m_stateManager->GetState(GameState::MAIN_MENU);
	delete m_stateManager->GetState(GameState::GAME_PLAY);
	delete m_stateManager->GetState(GameState::GAME_OVER);
}
