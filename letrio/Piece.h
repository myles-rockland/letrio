#pragma once
#include <iostream>
#include <vector>
#include <random>
#include "Constants.h"

class Piece
{
public:
	Piece(std::default_random_engine& generator, std::discrete_distribution<int> distribution);
	bool Drop(const char grid[GRID_HEIGHT][GRID_WIDTH]);
	bool MoveLeft(const char grid[GRID_HEIGHT][GRID_WIDTH]); // Might need to return a bool to say whether it was successful
	bool MoveRight(const char grid[GRID_HEIGHT][GRID_WIDTH]); // "
	void DropInstantly(char grid[GRID_HEIGHT][GRID_WIDTH]);
	bool Rotate(const char grid[GRID_HEIGHT][GRID_WIDTH], const bool clockwise = true); // Might need to return a bool, since rotation can fail in some cases. Rotation is around character 2. Use with currentPiece
	void Fix(char grid[GRID_HEIGHT][GRID_WIDTH]) const; // Fixes the piece to the grid. Use with currentPiece
	void ShuffleLetters(); // Shuffle letter positions. Use with currentPiece
	bool ChangeShape(const char grid[GRID_HEIGHT][GRID_WIDTH]); // Change shape. Use with currentPiece
	bool IsOverlapping(const char grid[GRID_HEIGHT][GRID_WIDTH]) const;
	char GetCharacter(const int index) const;
	int positions[3][2];
private:
	enum Orientation { Up, Right, Down, Left } orientation; // Can start in any orientation
	enum Shape { L, Line } shape; // Can start in any shape
	char characters[3];
};

