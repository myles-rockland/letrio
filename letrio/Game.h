#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "irrKlang.h"
#include "Constants.h"
#include "Piece.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

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
	void SaveHighScore() const;

	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	irrklang::ISoundEngine* engine;
	irrklang::ISound* bgMusic;
	std::default_random_engine generator;
	std::discrete_distribution<int> distribution;
	unordered_set<string> validWords;
	vector<string> wordsMade;
	bool isRunning;
	bool isPaused;
	bool gameOver;
	char grid[GRID_HEIGHT][GRID_WIDTH];
	Piece currentPiece;
	Piece nextPiece;
	int currentScore;
	int highScore;
	bool instantDropped;
	float speed;
	Uint64 lastDropTicks;
	int level;
};
