#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "Constants.h"
#include "Piece.h"

class Game
{
public:
	Game();
	~Game();
	void Run();
	bool IsRunning() const { return isRunning; }
private:
	void HandleInput();
	void Update();
	void Render();

	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	bool isRunning;
	bool gameOver;
	char grid[GRID_HEIGHT][GRID_WIDTH];
	Piece currentPiece;
	Piece nextPiece;
	int score;
	int speed;
	int level;
	// Game will need a way to save high score...
	// Also needs a dictionary of words
};
