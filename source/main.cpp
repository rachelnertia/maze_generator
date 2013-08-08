#include "SDL/SDL.h"
#include <stdlib.h>
#include <time.h>

// size of maze
#define ROWS	20
#define COLUMNS	30

// enum to keep track of the 4 directions
//enum DIR { NORTH, SOUTH, EAST, WEST};



int main(int argc, char* args[]) {
	// initialise random number generator
	srand(time(NULL));

	// bool representing whether or not the user has Xed out the window
	bool quit = false;

	// Surfaces:
	SDL_Surface* screen = NULL;

	// Event structure:
	SDL_Event event;

	// Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		return 1;
	}

	// set up the screen surface as a surface in system memory
	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

	// 
	while(!quit) {
		// while there's an event to handle
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					quit = true;
					break;
			}
		}
	}


	// Quit SDL
	SDL_Quit();

	return 0;
}