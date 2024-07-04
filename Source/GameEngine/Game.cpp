#pragma once

#include "Game.h"
#include <GL/glew.h>
#include <time.h>

using namespace GameEngine;

Game::Game()
{
	m_window = nullptr;
	m_glcontext = NULL;

	// empty ctor to ensure that members are created after the OpenGL context is set up
}
	
/// <summary>
/// Creates an SDL2 game windows with an OpenGL context
/// </summary>
/// <returns>true if success, and false otherwise</returns>
bool Game::Initialize()
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Could not initialize SDL. ErrorCode=%s\n", SDL_GetError());
		return false;
	}

	// Create the window and OpenGL context.
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); // use 32-bit buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // enable double buffering

	m_window = SDL_CreateWindow(
	"Kaboom Typer",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	Common::WINDOW_WIDTH, Common::WINDOW_HEIGHT,
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
	glViewport(0, 0, Common::WINDOW_WIDTH, Common::WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);

	//define a 2D orthographic projection matrix
	const double NEAR_CLIP = 0.0;
	const double FAR_CLIP = 100.0; // could use smaller range for 2D rendering
	glOrtho(0, Common::WINDOW_WIDTH, Common::WINDOW_HEIGHT, 0, NEAR_CLIP, FAR_CLIP);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	srand(time(NULL));

	// Load the game objects
	LoadData();

	return true;
}

/// <summary>
/// Load game assets and objects
/// </summary>
void Game::LoadData()
{
	// Load GameManager
	m_gameManager = GameManager::Create(); // must use the factory method
}

void Game::RunLoop()
{
	while (!m_shouldExit)
	{
		ProcessInput();
		Update();
		Draw();
	}
}

void Game::ProcessInput()
{
	m_shouldExit = m_gameManager->GetExitGame();
}

void Game::Update()
{
	// Compute deltaTime - the time difference between each frame
	m_currentTime = SDL_GetTicks();
	m_deltaTime = (m_currentTime - m_previousTime) / 1000.0f;
	m_previousTime = m_currentTime;

	// Calculate FPS and print
	if (m_currentTime > m_f_previousTime + 1000)
	{
		printf("FPS: %i\n", m_fps);
		m_fps = 0;
		m_f_previousTime = m_currentTime;
	}

	m_fps++; // increment frame counter each iteration

	m_gameManager->Update(m_deltaTime);
}

void Game::Draw()
{
	// Draw Frame
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this

	// Draw Game
	m_gameManager->Render();

	// Swap Window
	SDL_GL_SwapWindow(m_window);
}

void Game::Shutdown()
{
	// first delete OpenGL context then SDL
	SDL_GL_DeleteContext(m_glcontext);
	SDL_Quit();
}
