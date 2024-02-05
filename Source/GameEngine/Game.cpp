#include"Game.h"
#include<GL/glew.h>
#include<time.h>
#include "Sprite.h"
#include "TextString.h"
#include "TextStringFont.h"
#include "TextBlock.h"
#include "DrawUtils.h"
#include "Utilities.h"

using namespace GameEngine;
using namespace GameEngine::Utility;

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
	m_InputManager->ProcessInput();
	m_shouldExit = m_InputManager->ShouldQuit();
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

	m_sprite->Update();
	m_textStr->Update(m_deltaTime);

	//fmod_sys->update(); // If you don't update the sound will play once
}

void Game::GenerateOutput()
{
	// Draw Frame
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this

	// Draw Objects
	m_textBlock->Draw();
	m_textStr->DrawText();
	m_textStr2->DrawText();

	// Swap Window
	SDL_GL_SwapWindow(m_window);
}

void Game::LoadData()
{
	// Initialize Input Manager
	m_InputManager = new InputManager();
	m_sprite = new Sprite();
	m_InputManager->RegisterObserver(m_sprite);

	// Initialize a TextString to drawing
	m_textStr = new TextString();
	m_textStr2 = new TextString();

	Utilities::ReadXmlFile("../../Config/FontParameters.xml", m_fontParameters); // TODO: DON'T USE HARD-CODED PATHS

	TextStringFont font;
	font.image = m_fontParameters.m_texture;
	font.imageWidth = m_fontParameters.m_fontsheetWidth;
	font.imageHeight = m_fontParameters.m_fontsheetHeight;
	font.frameWidth = m_fontParameters.m_fontWidth;
	font.frameHeight = m_fontParameters.m_fontHeight;
	int x = 8; // x position to draw on screen
	int y = 4; // y position to draw on screen
	m_textStr->Initialize("Kaboom Typer!", x, y, font);
	m_textStr2->Initialize("More Text", x + 50, y + 50, font);

	Utilities::ReadXmlFile("../../Config/TextBlockParameters.xml", m_textBlockParameters); // TODO: DON'T USE HARD-CODED PATHS

	// Load all the textures into the String-to-Image map
	m_stringToImageMap["red"] = m_textBlockParameters.m_redBlockTexture;
	m_stringToImageMap["blue"] = m_textBlockParameters.m_blueBlockTexture;
	m_stringToImageMap["green"] = m_textBlockParameters.m_greenBlockTexture;
	m_stringToImageMap["yellow"] = m_textBlockParameters.m_yellowBlockTexture;
	m_stringToImageMap["purple"] = m_textBlockParameters.m_purpleBlockTexture;
	m_stringToImageMap["white"] = m_textBlockParameters.m_whiteBlockTexture;
	m_stringToImageMap["orange"] = m_textBlockParameters.m_orangeBlockTexture;
	m_textBlockWidth = m_textBlockParameters.m_blockWidth;
	m_textBlockHeight = m_textBlockParameters.m_blockHeight;

	// Create a TextBlock
	std::string stringForTextBlock("Kaboom!");
	m_textBlock = new TextBlock(10, 10, m_textBlockParameters.m_blockWidth, m_textBlockParameters.m_blockHeight, stringForTextBlock.c_str(), m_stringToImageMap);
}

void Game::UnloadData()
{
	delete m_InputManager;
	delete m_sprite;
	delete m_textStr;
	delete m_textStr2;
	delete m_textBlock;
}

