#ifndef CELL_H
#define CELL_H

#define NORTH	0
#define EAST	1
#define SOUTH	2
#define WEST	3

class Cell {
private:
	bool visited;
	// array of bools representing which directions the cell is 'open'
	// that is, has no wall
	bool connectedDirections[4];

public:
	Cell();
	//
	bool isVisited() { return visited; };
	//
	bool isConnectedInDir(int);
	//
	// connect this cell with a neighbouring cell
	void makeConnection(int);

};


#endif