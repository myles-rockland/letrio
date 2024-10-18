#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "Constants.h"
#include "Piece.h"
#include <vector>
#include <unordered_map>

using namespace std;

class Game
{
public:
	Game();
	void CleanUp();
	void Run();
	bool IsRunning() const { return isRunning; }
private:
	void HandleInput();
	void Update();
	void Render();
	void CheckWords();
	bool ValidateWord(const string word);
	void UpdateScore(const string word);

	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	Uint64 startTicks;
	unordered_map<string, string> validWords;
	int drops;
	bool isRunning;
	bool gameOver;
	char grid[GRID_HEIGHT][GRID_WIDTH];
	Piece currentPiece;
	Piece nextPiece;
	int currentScore;
	int highScore;
	int wordsMade;
	float speed;
	int level;
	// Game will need a way to save high score...
	// Also needs a dictionary of words
};
