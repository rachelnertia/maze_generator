#include "SDL/SDL.h"
#include <stdlib.h>
#include <time.h>
#include "cell.h"
#include <vector>
#include <stack>
#include <string>
#include <fstream>

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define BPP	32

// size of maze
#define ROWS	12
#define COLUMNS	12

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


int GetNumberOfUnvisitedCells(Cell [ROWS][COLUMNS]);
void DepthFirstSearch(Cell [ROWS][COLUMNS]);
std::vector<GridRef> GetUnvisitedNeighbours(Cell [ROWS][COLUMNS], GridRef);
void InitGrid(Cell [ROWS][COLUMNS]);

//void drawGrid(Cell [ROWS][COLUMNS]);

void DrawGrid(Cell [ROWS][COLUMNS], SDL_Surface*);



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
	Cell grid [ROWS][COLUMNS];
	log("setting up grid");
	InitGrid(grid);

	
	// cut the maze
	log("beginning depth first search maze generation");
	DepthFirstSearch(grid);

	// draw the maze
	log("drawing");
	DrawGrid(grid, screen);

	//SDL_BlitSurface(drawnMaze, NULL, screen, NULL);
	SDL_Flip(screen);

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


void InitGrid(Cell grid[ROWS][COLUMNS]) {
	for (int x = 0; x < ROWS; x++) {
		for (int y = 0; y < COLUMNS; y++) {
			grid[x][y] = Cell(x, y);
		}
	}
}


// Make a maze out of a given grid of Cells using a Depth-first Search algorithm
// implemented using backtracking
void DepthFirstSearch(Cell grid[ROWS][COLUMNS]) {
	// start at a random cell and call it the exit
	GridRef exitCell(rand()%ROWS, rand()%COLUMNS);

	// make the initial cell the current cell and mark it as visited
	GridRef currentCell = exitCell;
	grid[currentCell.x][currentCell.y].visited = true;

	std::stack<Cell> stack;

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
			if (chosenCell.x > currentCell.x) {
				grid[currentCell.x][currentCell.y].connectedDirections[EAST] = 1;
				grid[chosenCell.x][chosenCell.y].connectedDirections[WEST] = 1;
			} else if (chosenCell.x < currentCell.x) {
				grid[currentCell.x][currentCell.y].connectedDirections[WEST] = 1;
				grid[chosenCell.x][chosenCell.y].connectedDirections[EAST] = 1;
			} else if (chosenCell.y > currentCell.y) {
				grid[currentCell.x][currentCell.y].connectedDirections[SOUTH] = 1;
				grid[chosenCell.x][chosenCell.y].connectedDirections[NORTH] = 1;
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
			currentCell = GridRef(rand()%ROWS, rand()%COLUMNS);
			grid[currentCell.x][currentCell.y].visited = true;
		}

		log("logging stack size:");
		log(stack.size());
	}

}

int GetNumberOfUnvisitedCells(Cell grid[ROWS][COLUMNS]) {
	int n = 0;

	for (int x = 0; x < ROWS; x++) {
		for (int y = 0; y < COLUMNS; y++) {
			if (grid[x][y].visited == false)
				n++;
		}
	}

	log("logging number of unvisited cells:");
	log(n);
	return n;
}

std::vector<GridRef> GetUnvisitedNeighbours(Cell grid[ROWS][COLUMNS], GridRef cell) {
	std::vector<GridRef> list;
	if (cell.x > 1) {
		// check the cell to the west/left
		if ( (grid[cell.x - 1][cell.y]).visited == false ) {
			// add it to the list
			list.push_back( GridRef(cell.x - 1, cell.y) );
		}
	}
	if (cell.y > 1) {
		// check the cell on top/to the north
		if ( (grid[cell.x][cell.y - 1]).visited == false ) {
			list.push_back( GridRef(cell.x, cell.y - 1) );
		}
	}
	if (cell.x < ROWS) {
		// to the right/east
		if ( (grid[cell.x + 1][cell.y]).visited == false ) {
			list.push_back( GridRef(cell.x + 1, cell.y) );
		}
	}
	if (cell.y < COLUMNS) {
		// check the cell on top/to the north
		if ( (grid[cell.x][cell.y + 1]).visited == false ) {
			list.push_back( GridRef(cell.x, cell.y + 1) );
		}
	}

	return list;
}


void DrawGrid(Cell grid[ROWS][COLUMNS], SDL_Surface* surface) {
	// fill black
	// rect struct for filling
	SDL_Rect fillRect;
	fillRect.x = 0;
	fillRect.y = 0;
	fillRect.w = SCREEN_WIDTH;
	fillRect.h = SCREEN_HEIGHT;
	SDL_FillRect(surface, &fillRect, 0x000000);

	for (int x = 0; x < ROWS; x++) {
		for (int y = 0; y < COLUMNS; y++) {
			// fill a rectangle in the middle
			if (grid[x][y].visited) {
				fillRect.x = x*(SCREEN_WIDTH/COLUMNS) + ((SCREEN_WIDTH/COLUMNS)/4);
				fillRect.y = y*(SCREEN_HEIGHT/ROWS) + ((SCREEN_HEIGHT/ROWS)/4);
				fillRect.w = (SCREEN_WIDTH/COLUMNS)/2;
				fillRect.h = (SCREEN_HEIGHT/ROWS)/2;
				SDL_FillRect(surface, &fillRect, 0xFFFFFF);
			}
			// draw connections
			if (grid[x][y].connectedDirections[NORTH]) {
				fillRect.x = x*(SCREEN_WIDTH/COLUMNS) + ((SCREEN_WIDTH/COLUMNS)/4);
				fillRect.y = y*(SCREEN_HEIGHT/ROWS);
				fillRect.w = (SCREEN_WIDTH/COLUMNS)/2;
				fillRect.h = (SCREEN_HEIGHT/ROWS)/4;
				SDL_FillRect(surface, &fillRect, 0xFFFFFF);
			}
			if (grid[x][y].connectedDirections[SOUTH]) {
				fillRect.x = x*(SCREEN_WIDTH/COLUMNS) + ((SCREEN_WIDTH/COLUMNS)/4);
				fillRect.y = y*(SCREEN_HEIGHT/ROWS) + (((SCREEN_HEIGHT/ROWS)/4)*3);
				fillRect.w = (SCREEN_WIDTH/COLUMNS)/2;
				fillRect.h = (SCREEN_HEIGHT/ROWS)/4;
				SDL_FillRect(surface, &fillRect, 0xFFFFFF);
			}
			if (grid[x][y].connectedDirections[WEST]) {
				fillRect.x = x*(SCREEN_WIDTH/COLUMNS);
				fillRect.y = y*(SCREEN_HEIGHT/ROWS) + ((SCREEN_HEIGHT/ROWS)/4);
				fillRect.w = (SCREEN_WIDTH/COLUMNS)/4;
				fillRect.h = (SCREEN_HEIGHT/ROWS)/2;
				SDL_FillRect(surface, &fillRect, 0xFFFFFF);
			}			
			if (grid[x][y].connectedDirections[EAST]) {
				fillRect.x = x*(SCREEN_WIDTH/COLUMNS) + (((SCREEN_WIDTH/COLUMNS)/4)*3);
				fillRect.y = y*(SCREEN_HEIGHT/ROWS) + ((SCREEN_HEIGHT/ROWS)/4);
				fillRect.w = (SCREEN_WIDTH/COLUMNS)/4;
				fillRect.h = (SCREEN_HEIGHT/ROWS)/2;
				SDL_FillRect(surface, &fillRect, 0xFFFFFF);
			}
		}
	}

}