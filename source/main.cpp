#include "SDL/SDL.h"
#include <stdlib.h>
#include <time.h>
#include "cell.h"
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <iostream>

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define BPP	32

// size of maze
#define GRID_WIDTH	20
#define GRID_HEIGHT	20


// Event structure:
SDL_Event event;
bool quit;

//File stream for the game log.
std::ofstream logger( "log.txt" );
void log( std::string message )
{
    //Write message to file
    logger << message << std::endl;

    //Flush the buffer
    logger.flush();
}
void log(int n) {
	logger << n << std::endl;
	logger.flush();
}


int GetNumberOfUnvisitedCells(Cell [GRID_WIDTH][GRID_HEIGHT]);
void DepthFirstSearch(Cell [GRID_WIDTH][GRID_HEIGHT], SDL_Surface*);
std::vector<GridRef> GetUnvisitedNeighbours(Cell [GRID_WIDTH][GRID_HEIGHT], GridRef);
void InitGrid(Cell [GRID_WIDTH][GRID_HEIGHT]);

//void drawGrid(Cell [GRID_WIDTH][GRID_HEIGHT]);

void DrawGrid(Cell [GRID_WIDTH][GRID_HEIGHT], SDL_Surface*);



int main(int argc, char* args[]) {
	// initialise random number generator
	srand(time(NULL));

	// bool representing whether or not the user has Xed out the window
	quit = false;

	// Surfaces:
	SDL_Surface* screen = NULL;
	

	// Initialise SDL
	log("initialising SDL");
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		return 1;
	}

	log("setting up screen");
	// set up the screen surface as a surface in system memory
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BPP, SDL_SWSURFACE);
	// Set the window caption
	SDL_WM_SetCaption("amaze generator", NULL);

	// maze grid
	Cell grid [GRID_WIDTH][GRID_HEIGHT];
	log("setting up grid");
	InitGrid(grid);

	
	// cut the maze
	log("beginning depth first search maze generation");
	DepthFirstSearch(grid, screen);

	// draw the maze
	log("drawing");
	DrawGrid(grid, screen);

	//SDL_BlitSurface(drawnMaze, NULL, screen, NULL);
	//SDL_Flip(screen);

	// 
	log("beginning while loop");
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

	// clean up
	//SDL_FreeSurface(drawnMaze);

	log("quitting SDL");
	// Quit SDL
	SDL_Quit();

	return 0;
}


void InitGrid(Cell grid[GRID_WIDTH][GRID_HEIGHT]) {
	for (int x = 0; x < GRID_WIDTH; x++) {
		for (int y = 0; y < GRID_HEIGHT; y++) {
			grid[x][y] = Cell(x, y);
		}
	}
}


// Make a maze out of a given grid of Cells using a Depth-first Search algorithm
// implemented using backtracking
void DepthFirstSearch(Cell grid[GRID_WIDTH][GRID_HEIGHT], SDL_Surface* surface) {
	// start at a random cell and call it the exit
	GridRef exitCell(rand()%GRID_WIDTH, rand()%GRID_HEIGHT);

	// make the initial cell the current cell and mark it as visited
	GridRef currentCell = exitCell;
	grid[currentCell.x][currentCell.y].visited = true;

	std::stack<Cell> stack;

	//bool advance = false;

	// while there are unvisited cells
	while (GetNumberOfUnvisitedCells(grid) > 0) {
		// if the current cell has any neighbours which have not been visited
		std::vector<GridRef> unvisitedNeighbours = GetUnvisitedNeighbours(grid, currentCell);
		if (unvisitedNeighbours.empty() == false) {
			// choose randomly one of the unvisited neighbours
			int r = rand()%unvisitedNeighbours.size();
			GridRef chosenCell = unvisitedNeighbours[r];

			// push the current cell to the stack
			stack.push(grid[currentCell.x][currentCell.y]);

			// remove the wall between the current cell and the chosen cell
			// if the chosen cell is east of the current cell
			if (chosenCell.x > currentCell.x) {
				grid[currentCell.x][currentCell.y].connectedDirections[EAST] = 1;
				grid[chosenCell.x][chosenCell.y].connectedDirections[WEST] = 1;
			// if the chosen cell is west of the current cell
			} else if (chosenCell.x < currentCell.x) {
				grid[currentCell.x][currentCell.y].connectedDirections[WEST] = 1;
				grid[chosenCell.x][chosenCell.y].connectedDirections[EAST] = 1;
			// if the chosen cell is south of the current cell
			} else if (chosenCell.y > currentCell.y) {
				grid[currentCell.x][currentCell.y].connectedDirections[SOUTH] = 1;
				grid[chosenCell.x][chosenCell.y].connectedDirections[NORTH] = 1;
			// if the chosen cell is north of the current cell
			} else if (chosenCell.y < currentCell.y) {
				grid[currentCell.x][currentCell.y].connectedDirections[NORTH] = 1;
				grid[chosenCell.x][chosenCell.y].connectedDirections[SOUTH] = 1;
			}

			// make the chosen cell the current cell and mark it as visited
			currentCell = chosenCell;
			grid[currentCell.x][currentCell.y].visited = true;

		// else if stack is not empty
		} else if (stack.empty() == false) {
			// pop a cell from the stack
			Cell topCell = stack.top();
			stack.pop();
			// make it the current cell
			currentCell = GridRef(topCell.gridRef.x, topCell.gridRef.y);
		} else {
			// pick a random cell, make it the current cell and mark it as visited
			currentCell = GridRef(rand()%GRID_WIDTH, rand()%GRID_HEIGHT);
			grid[currentCell.x][currentCell.y].visited = true;
		}

		DrawGrid(grid, surface);
		SDL_Delay(5);

		/*advance = false;

		while (advance == false) {
			while(SDL_PollEvent(&event)) {
				switch(event.type) {
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_RETURN) {
							advance = true;
						}
						break;
					case SDL_QUIT:
						return;
				}
			}
		}*/

	}

}

int GetNumberOfUnvisitedCells(Cell grid[GRID_WIDTH][GRID_HEIGHT]) {
	int n = 0;

	for (int x = 0; x < GRID_WIDTH; x++) {
		for (int y = 0; y < GRID_HEIGHT; y++) {
			if (grid[x][y].visited == false)
				n++;
		}
	}

	return n;
}

std::vector<GridRef> GetUnvisitedNeighbours(Cell grid[GRID_WIDTH][GRID_HEIGHT], GridRef cell) {
	std::vector<GridRef> list;
	
	if (cell.x > 0) {
		// check the cell to the west/left
		if ( (grid[cell.x - 1][cell.y]).visited == false ) {
			// add it to the list
			list.push_back( GridRef(cell.x - 1, cell.y) );
		}
	}
	if (cell.x < GRID_WIDTH - 1) {
		// to the right/east
		if ( (grid[cell.x + 1][cell.y]).visited == false ) {
			list.push_back( GridRef(cell.x + 1, cell.y) );
		}
	}
	if (cell.y > 0) {
		// check the cell on top/to the north
		if ( (grid[cell.x][cell.y - 1]).visited == false ) {
			list.push_back( GridRef(cell.x, cell.y - 1) );
		}
	}
	if (cell.y < GRID_HEIGHT -1) {
		// check the cell on top/to the north
		if ( (grid[cell.x][cell.y + 1]).visited == false ) {
			list.push_back( GridRef(cell.x, cell.y + 1) );
		}
	}

	/*for (int i = 0; i <list.size(); i++) {
		if ((list[i].x < 0) || (list[i].x > GRID_WIDTH)
			|| (list[i].y < 0) || (list[i].y > GRID_HEIGHT)) {
			log("a cell is out of bounds");
		} 
	}*/
	
	//log(list.size());
	return list;
}


void DrawGrid(Cell grid[GRID_WIDTH][GRID_HEIGHT], SDL_Surface* surface) {
	// fill black
	// rect struct for filling
	SDL_Rect fillRect;
	fillRect.x = 0;
	fillRect.y = 0;
	fillRect.w = SCREEN_WIDTH;
	fillRect.h = SCREEN_HEIGHT;
	SDL_FillRect(surface, &fillRect, 0x000000);

	for (int x = 0; x < GRID_WIDTH; x++) {
		for (int y = 0; y < GRID_HEIGHT; y++) {
			// fill a rectangle in the middle
			if (grid[x][y].visited) {
				fillRect.x = x*(SCREEN_WIDTH/GRID_WIDTH) + ((SCREEN_WIDTH/GRID_WIDTH)/4);
				fillRect.y = y*(SCREEN_HEIGHT/GRID_HEIGHT) + ((SCREEN_HEIGHT/GRID_HEIGHT)/4);
				fillRect.w = (SCREEN_WIDTH/GRID_WIDTH)/2;
				fillRect.h = (SCREEN_HEIGHT/GRID_HEIGHT)/2;
				SDL_FillRect(surface, &fillRect, 0xFFFFFF);
			}
			// draw connections
			if (grid[x][y].connectedDirections[NORTH]) {
				fillRect.x = x*(SCREEN_WIDTH/GRID_WIDTH) + ((SCREEN_WIDTH/GRID_WIDTH)/4);
				fillRect.y = y*(SCREEN_HEIGHT/GRID_HEIGHT);
				fillRect.w = (SCREEN_WIDTH/GRID_WIDTH)/2;
				fillRect.h = (SCREEN_HEIGHT/GRID_HEIGHT)/4;
				SDL_FillRect(surface, &fillRect, 0xFFFFFF);
			}
			if (grid[x][y].connectedDirections[SOUTH]) {
				fillRect.x = x*(SCREEN_WIDTH/GRID_WIDTH) + ((SCREEN_WIDTH/GRID_WIDTH)/4);
				fillRect.y = y*(SCREEN_HEIGHT/GRID_HEIGHT) + (((SCREEN_HEIGHT/GRID_HEIGHT)/4)*3);
				fillRect.w = (SCREEN_WIDTH/GRID_WIDTH)/2;
				fillRect.h = (SCREEN_HEIGHT/GRID_HEIGHT)/4;
				SDL_FillRect(surface, &fillRect, 0xFFFFFF);
			}
			if (grid[x][y].connectedDirections[WEST]) {
				fillRect.x = x*(SCREEN_WIDTH/GRID_WIDTH);
				fillRect.y = y*(SCREEN_HEIGHT/GRID_HEIGHT) + ((SCREEN_HEIGHT/GRID_HEIGHT)/4);
				fillRect.w = (SCREEN_WIDTH/GRID_WIDTH)/4;
				fillRect.h = (SCREEN_HEIGHT/GRID_HEIGHT)/2;
				SDL_FillRect(surface, &fillRect, 0xFFFFFF);
			}			
			if (grid[x][y].connectedDirections[EAST]) {
				fillRect.x = x*(SCREEN_WIDTH/GRID_WIDTH) + (((SCREEN_WIDTH/GRID_WIDTH)/4)*3);
				fillRect.y = y*(SCREEN_HEIGHT/GRID_HEIGHT) + ((SCREEN_HEIGHT/GRID_HEIGHT)/4);
				fillRect.w = (SCREEN_WIDTH/GRID_WIDTH)/4;
				fillRect.h = (SCREEN_HEIGHT/GRID_HEIGHT)/2;
				SDL_FillRect(surface, &fillRect, 0xFFFFFF);
			}
		}
	}

	SDL_Flip(surface);

}