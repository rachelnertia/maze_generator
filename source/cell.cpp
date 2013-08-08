#include "cell.h"

Cell::Cell() {
	visited = false;
	// cell begins with walls in all directions
	for (int i = 0; i < 4; i++) {
		connectedDirections[i] = 0;
	}
}

bool Cell::isConnectedInDir(int direction) {
	return connectedDirections[direction];
}

void Cell::makeConnection(int direction) {
	connectedDirections[direction] = 1;
}