#ifndef CELL_H
#define CELL_H

#define NORTH	0
#define EAST	1
#define SOUTH	2
#define WEST	3

struct GridRef {
	int x, y;
	GridRef() {x = 0; y= 0;};
	GridRef(int _x, int _y) { x = _x; y = _y; };
};

struct Cell {
	GridRef gridRef;
	bool visited;
	// array of bools representing which directions the cell is 'open'
	// that is, has no wall
	bool connectedDirections[4];
	//
	Cell();
	Cell(int, int);
};


#endif