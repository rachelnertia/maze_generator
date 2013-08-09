#include "cell.h"

Cell::Cell() {
	gridRef.x = 0;
	gridRef.y = 0;
	visited = false;
	// cell begins with walls in all directions
	for (int i = 0; i < 4; i++) {
		connectedDirections[i] = 0;
	}
}

Cell::Cell(int _x, int _y) {
	gridRef.x = _x;
	gridRef.y = _y;
	visited = false;
	// cell begins with walls in all directions
	for (int i = 0; i < 4; i++) {
		connectedDirections[i] = 0;
	}
}