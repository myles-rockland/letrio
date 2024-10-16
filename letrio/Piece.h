#pragma once
#include "Constants.h"

class Piece
{
public:
	Piece();
	void MoveLeft(const char grid[GRID_HEIGHT][GRID_WIDTH]); // Might need to return a bool to say whether it was successful
	void MoveRight(const char grid[GRID_HEIGHT][GRID_WIDTH]); // "
	void DropInstantly(char grid[GRID_HEIGHT][GRID_WIDTH]);
	void Rotate(const char grid[GRID_HEIGHT][GRID_WIDTH], const bool clockwise = true); // Might need to return a bool, since rotation can fail in some cases. Rotation is around character 2. Use with currentPiece
	void Fix(char grid[GRID_HEIGHT][GRID_WIDTH]); // Fixes the piece to the grid. Use with currentPiece
	void ShuffleLetters(); // Shuffle letter positions. Use with currentPiece
	void ChangeShape(const char grid[GRID_HEIGHT][GRID_WIDTH]); // Change shape. Use with currentPiece
	void ChangeLetters(); // Change the letters. Use with nextPiece
private:
	enum Orientation { Up, Right, Down, Left } orientation; // Can start in any orientation
	enum Shape { L, Line } shape; // Can start in any shape
	char characters[3];
	int positions[3][2];
};

