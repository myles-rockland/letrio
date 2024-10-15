#include <iostream>
#include "Game.h"

using namespace std;

Game::Game() : isRunning(true), gameOver(false), window(nullptr), renderer(nullptr), font(nullptr), score(0), speed(START_SPEED), level(START_LEVEL)
{
    // Initialise SDL subsystems
    // Might need to init audio system as well
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cerr << "SDL failed to initialise. SDL_Error: " << SDL_GetError() << endl;
        isRunning = false;
        return;
    }

    if (TTF_Init() < 0)
    {
        cerr << "SDL_ttf failed to initialise. TTF_Error: " << TTF_GetError() << endl;
        isRunning = false;
        return;
    }

    // Initialising SDL member variables
    window = SDL_CreateWindow("Letrio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        cerr << "Window couldn't be created. SDL_Error: " << SDL_GetError() << endl;
        isRunning = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        cerr << "Renderer couldn't be created. SDL_Error: " << SDL_GetError() << endl;
        isRunning = false;
        return;
    }

    font = TTF_OpenFont("KarenFat.ttf", FONT_SIZE);
    if (font == nullptr)
    {
        cerr << "Font couldn't be opened. TTF_Error: " << TTF_GetError() << endl;
        isRunning = false;
        return;
    }

    // Initialise the grid
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            grid[i][j] = ' ';
        }
    }
}

Game::~Game()
{
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::Run()
{
    while (isRunning) // Makes the IsRunning method useless?
    {
        HandleInput();
        Update();
        Render();
    }
}

void Game::HandleInput()
{
    SDL_Event event;
    const Uint8* state = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&event))
    {
        if (state[SDL_SCANCODE_ESCAPE] || event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        else
        {
            if (state[SDL_SCANCODE_LEFT])
            {
                currentPiece.MoveLeft(grid);
            }
            if (state[SDL_SCANCODE_RIGHT])
            {
                currentPiece.MoveRight(grid);
            }
            if (state[SDL_SCANCODE_UP]) // Instant Drop
            {
                currentPiece.DropInstantly(grid);
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_DOWN) // Fast Drop
            {
                speed /= 1.3;
            }
            if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_DOWN) // Reset speed after fast drop
            {
                speed *= 1.3; // Might need to base this on start speed... don't want floating point errors
            }
            if (state[SDL_SCANCODE_Z])
            {
                currentPiece.Rotate(grid, false);
            }
            if (state[SDL_SCANCODE_X])
            {
                currentPiece.Rotate(grid);
            }
            if (state[SDL_SCANCODE_C])
            {
                currentPiece.ShuffleLetters();
            }
            if (state[SDL_SCANCODE_SPACE])
            {
                currentPiece.ChangeShape(grid);
            }
        }
    }
}

void Game::Update()
{

}

void Game::Render()
{
    // Draw the grid
    SDL_Delay(50);
}