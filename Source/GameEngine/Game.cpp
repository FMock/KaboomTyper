#pragma once

#include "Game.h"
#include "DrawUtils.h"
#include <GL/glew.h>
#include <time.h>

using namespace GameEngine;
using namespace DrawUtilities;

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

	// Physical window is the logical play field plus a border margin on every side, so the
	// red border (drawn in Draw()) is extra space rather than covering existing content.
	const int physicalWidth = Common::WINDOW_WIDTH + 2 * Common::BORDER_THICKNESS;
	const int physicalHeight = Common::WINDOW_HEIGHT + 2 * Common::BORDER_THICKNESS;

	m_window = SDL_CreateWindow(
	"Kaboom Typer",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	physicalWidth, physicalHeight,
	SDL_WINDOW_OPENGL);

	if (!m_window)
	{
		fprintf(stderr, "Could not create window. ErrorCode=%s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

	// Create an OpenGL context associated with the window.
	m_glcontext = SDL_GL_CreateContext(m_window);
	if (!m_glcontext)
	{
		fprintf(stderr, "Could not create OpenGL context. ErrorCode=%s\n", SDL_GetError());
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
		SDL_Quit();
		return false;
	}

	// Make sure we have a recent version of OpenGL.
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		fprintf(stderr, "Could not initialize glew. ErrorCode=%s\n", glewGetErrorString(glewError));
		SDL_GL_DeleteContext(m_glcontext);
		m_glcontext = NULL;
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
		SDL_Quit();
		return false;
	}
	if (GLEW_VERSION_2_0)
	{
		fprintf(stderr, "OpenGL 2.0 or greater supported: Version = %s\n",
			glGetString(GL_VERSION));
	}
	else
	{
		fprintf(stderr, "OpenGL max supported version is too low.\n");
		SDL_GL_DeleteContext(m_glcontext);
		m_glcontext = NULL;
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
		SDL_Quit();
		return false;
	}

	// Setup OpenGL state.
	glViewport(0, 0, physicalWidth, physicalHeight);
	glMatrixMode(GL_PROJECTION);

	//define a 2D orthographic projection matrix. The visible range is expanded by the border
	//thickness on every side, so logical (0,0)..(WINDOW_WIDTH,WINDOW_HEIGHT) renders inset by the
	//border, leaving a margin around it for the red border. All UI keeps its logical coordinates.
	const double NEAR_CLIP = 0.0;
	const double FAR_CLIP = 100.0; // could use smaller range for 2D rendering
	const int B = Common::BORDER_THICKNESS;
	glOrtho(-B, Common::WINDOW_WIDTH + B, Common::WINDOW_HEIGHT + B, -B, NEAR_CLIP, FAR_CLIP);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	srand(static_cast<unsigned int>(time(NULL)));

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

	// Light gray border around the whole app, drawn last so it sits on top, in the margin
	// just outside the logical play field (which holds the existing blue border inside it).
	const int B = Common::BORDER_THICKNESS;
	const int W = Common::WINDOW_WIDTH;
	const int H = Common::WINDOW_HEIGHT;
	const RGBColor borderColor = RGBColor::GetRGBColor(RGBColor::LightGray);
	glDrawFilledRectangle(-B, -B, W + 2 * B, B, 1.0f, 1.0f, borderColor); // top
	glDrawFilledRectangle(-B, H, W + 2 * B, B, 1.0f, 1.0f, borderColor);  // bottom
	glDrawFilledRectangle(-B, 0, B, H, 1.0f, 1.0f, borderColor);          // left
	glDrawFilledRectangle(W, 0, B, H, 1.0f, 1.0f, borderColor);           // right

	// Swap Window
	SDL_GL_SwapWindow(m_window);
}

void Game::Shutdown()
{
	// first delete OpenGL context, then destroy the window, then quit SDL
	if (m_glcontext)
	{
		SDL_GL_DeleteContext(m_glcontext);
		m_glcontext = NULL;
	}
	if (m_window)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
	SDL_Quit();
}
