#include"Game.h"
#include<GL/glew.h>
#include<time.h>


Game::Game()
{
}

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

void Game::RunLoop()
{
	while (!m_shouldExit)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	UnloadData();
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(m_glcontext);
	SDL_Quit();
}

void Game::ProcessInput()
{
	m_kbState = SDL_GetKeyboardState(NULL);// We want status of all the keys

	memcpy(m_kbPrevState, m_kbState, sizeof(m_kbPrevState)); // copy key states into previous key states array

	// Handle OS message pump.
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_shouldExit = 1;
		}
	}

	// Take action if any keys are pressed. check SDL SCANCODES https://wiki.libsdl.org/SDL_Scancode
	if (m_kbState[SDL_SCANCODE_BACKSPACE] && !m_kbPrevState[SDL_SCANCODE_BACKSPACE])
	{
		m_noKeyPressTime = 0;
		printf("BACKSPACE\n");
	}
	else if (m_kbState[SDL_SCANCODE_RETURN] && !m_kbPrevState[SDL_SCANCODE_RETURN])
	{
		m_noKeyPressTime = 0;
		printf("RETURN\n");

	}
	else if (m_kbState[SDL_SCANCODE_LEFT] && !m_kbPrevState[SDL_SCANCODE_LEFT])
	{ // LEFT ARROW KEY PRESSED
		m_noKeyPressTime = 0;
		printf("LEFT_ARROW\n");

	}// NO DIRECTIONAL KEYS PRESSED
	else if (!m_kbState[SDL_SCANCODE_LEFT] &&									
		!m_kbPrevState[SDL_SCANCODE_LEFT] &&
		!m_kbState[SDL_SCANCODE_RIGHT] &&
		!m_kbPrevState[SDL_SCANCODE_RIGHT] &&
		!m_kbState[SDL_SCANCODE_UP] &&
		!m_kbPrevState[SDL_SCANCODE_UP] &&
		!m_kbState[SDL_SCANCODE_DOWN] &&
		!m_kbPrevState[SDL_SCANCODE_DOWN])
	{
		m_noKeyPressTime = 0;

	}
	else if (m_kbState[SDL_SCANCODE_RIGHT] && !m_kbPrevState[SDL_SCANCODE_RIGHT])
	{  // RIGHT ARROW KEY PRESSED
		m_noKeyPressTime = 0;
		printf("RIGHT_ARROW\n");
	}
	else if (m_kbState[SDL_SCANCODE_UP] && !m_kbPrevState[SDL_SCANCODE_UP])
	{  // UP ARROW KEY PRESSED
		m_noKeyPressTime = 0;
		printf("UP_ARROW\n");
	}
	else if (m_kbState[SDL_SCANCODE_DOWN] && !m_kbPrevState[SDL_SCANCODE_DOWN])
	{  // DOWN ARROW KEY PRESSED
		m_noKeyPressTime = 0;
		printf("DOWN_ARROW\n");
	}
	else if (m_kbState[SDL_SCANCODE_SPACE] && !m_kbPrevState[SDL_SCANCODE_SPACE])
	{
		m_noKeyPressTime = 0;
		printf("SPACE\n");
	}
	// player wants to exit game
	if (m_kbState[SDL_SCANCODE_ESCAPE])
	{
		m_shouldExit = 1;
	}
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
		//printf("fps: %i\n", fps);
		m_fps = 0;
		m_f_previousTime = m_f_currentTime;
	}

	m_fps++; // increment frame counter each iteration


	//fmod_sys->update(); // If you don't update the sound will play once
}

void Game::GenerateOutput()
{
	// Draw Frame
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this

	// Draw Objects

	// Swap Window
	SDL_GL_SwapWindow(m_window);
}

void Game::LoadData()
{

}

void Game::UnloadData()
{

}

