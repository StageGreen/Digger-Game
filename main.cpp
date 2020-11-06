#define SDL_MAIN_HANDLED

#include <iostream>
#include <SDL.h>
#include <random>
#include <chrono>

#include "Object.h"
#include "gameEngine.h"




int main(int argc, char *argv[])
{
	srand(time(NULL));

	GameEngine gameEngine;
	if (!gameEngine.InitEverything())
		return -1;

	gameEngine.RunGame();

	return 0;
}