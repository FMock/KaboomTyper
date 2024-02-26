#define SDL_MAIN_HANDLED
#include "Game.h"
#include "StateMachine.h" // do not delete

using namespace GameEngine;

/*  main.cpp
	Project Name - Kaboom Typer
	Author       - Frank Mock
	Description  - A refactor of a game I created in 2017
	Start Date   - 12/25/2023
	Last update  - 
*/

int main(int argc, char** argv)
{
	auto game = new Game();
	bool success = game->Initialize();
	if (success)
	{
		game->RunLoop();
	}
	game->Shutdown();

	delete game;
	return 0;
}