#include <iostream>
#include <fstream>
#include "Game.h"

using namespace std;

Game::Game() : isRunning(true), gameOver(false), window(nullptr), renderer(nullptr), font(nullptr), startTicks(0), currentScore(0), speed(START_SPEED), level(START_LEVEL), drops(0), wordsMade(0), highScore(0)
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

    startTicks = SDL_GetTicks64();

    // Read words from the text file into validWords
    ifstream wordsAlphaFile;
    try
    {
        wordsAlphaFile.open(WORDS_FILE_PATH);
        if (!wordsAlphaFile.is_open())
        {
            throw ios_base::failure("File could not be opened.");
        }
    }
    catch (const ios_base::failure& exception)
    {
        cerr << "An error has occured: " << exception.what() << endl;
        isRunning = false;
        return;
    }

    string word;
    while (getline(wordsAlphaFile, word))
    {
        if (word.size() >= MIN_WORD_SIZE)
        {
            for (auto& character : word) character = toupper(character); // Words have to be in upper case
            validWords.push_back(word);
        }
    }
    wordsAlphaFile.close();
    cout << "Valid words loaded";
}

void Game::CleanUp()
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
        Uint64 start = SDL_GetPerformanceCounter();
        HandleInput();
        Update();
        Render();
        Uint64 end = SDL_GetPerformanceCounter();
        float msElapsed = (end - start) / (float) SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(16.666f - msElapsed); // Cap the game to 60FPS
    }
    CleanUp();
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
            CleanUp();
            exit(0);
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
    // If the current piece overlaps with anything, game over
    if (currentPiece.IsOverlapping(grid))
    {
        gameOver = true;
    }
    
    // Drop the current piece every so often, based on speed
    Uint64 currentTicks = SDL_GetTicks64();
    Uint64 msSinceInit = currentTicks - startTicks;

    // If the current piece attempts to drop but is blocked, fix it in place
    if (msSinceInit / speed > drops) // Integer division
    {
        drops++;
        bool didDrop = currentPiece.Drop(grid);
        if (!didDrop)
        {
            currentPiece.Fix(grid);

            // Check for words and increase score/level/speed if necessary
            CheckWords();

            currentPiece = nextPiece;

            nextPiece.ChangeLetters();

            highScore = (currentScore > highScore) ? currentScore : highScore;
        }
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render grid
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i <= GRID_WIDTH; i++) // Vertical lines
    {
        float xPosition = i * CELL_LENGTH;
        SDL_RenderDrawLine(renderer, xPosition, 0, xPosition, WINDOW_HEIGHT);
    }
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        float yPosition = i * CELL_LENGTH;
        SDL_RenderDrawLine(renderer, 0, yPosition, CELL_LENGTH * GRID_WIDTH, yPosition);
    }

    // Render characters in grid
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            char gridValue = grid[j][i];
            if (gridValue != ' ')
            {
                int textX = (j * CELL_LENGTH) + ((CELL_LENGTH - FONT_SIZE) / 2);
                int textY = (i * CELL_LENGTH) + ((CELL_LENGTH - FONT_SIZE) / 2);
                SDL_Surface* surface = TTF_RenderGlyph_Solid(font, gridValue, { 255, 255, 255, 255 }); // Might need to change text colour
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                SDL_Rect textRect{ textX, textY, FONT_SIZE, FONT_SIZE };
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_DestroyTexture(texture);
            }
        }
    }

    // Render currentPiece and its characters using their positions
    for (int i = 0; i < 3; i++)
    {
        char character = currentPiece.GetCharacter(i);
        int textX = (currentPiece.positions[i][0] * CELL_LENGTH) + ((CELL_LENGTH - FONT_SIZE) / 2);
        int textY = (currentPiece.positions[i][1] * CELL_LENGTH) + ((CELL_LENGTH - FONT_SIZE) / 2);
        SDL_Surface* surface = TTF_RenderGlyph_Solid(font, character, { 255, 255, 255, 255 }); // Might need to change text colour
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect textRect{ textX, textY, FONT_SIZE, FONT_SIZE };
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_DestroyTexture(texture);
    }

    // Render nextPiece at top right
    /*
    for (int i = 0; i < 3; i++)
    {
        char character = nextPiece.GetCharacter(i);
        int textX = (nextPiece.positions[i][0] * CELL_LENGTH) + ((CELL_LENGTH - FONT_SIZE) / 2);
        int textY = (nextPiece.positions[i][1] * CELL_LENGTH) + ((CELL_LENGTH - FONT_SIZE) / 2);
        SDL_Surface* surface = TTF_RenderGlyph_Solid(font, character, { 255, 255, 255, 255 }); // Might need to change text colour
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect textRect{ textX, textY, FONT_SIZE, FONT_SIZE };
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_DestroyTexture(texture);
    }
    */

    // Render high score 

    // Render current score

    // Render level

    // Render wordsMade


    SDL_RenderPresent(renderer);
}

void Game::CheckWords()
{
    vector<string> foundWords;
    vector<vector<vector<int>>> positions; // A vector that stores a vector of "words". A vector of words stores a vector of character positions.
    
    // Only need to check rows/columns concerned with currentPiece, no need to go through the whole board
    int heightLowerBound = 999;
    int widthLowerBound = 999;
    for (int i = 0; i < 3; i++)
    {
        heightLowerBound = (currentPiece.positions[i][1] < heightLowerBound) ? currentPiece.positions[i][1] : heightLowerBound;
        widthLowerBound = (currentPiece.positions[i][0] < widthLowerBound) ? currentPiece.positions[i][0] : widthLowerBound;
    }
    int heightUpperBound = -1;
    int widthUpperBound = -1;
    for (int i = 0; i < 3; i++)
    {
        heightUpperBound = (currentPiece.positions[i][1] > heightUpperBound) ? currentPiece.positions[i][1] : heightUpperBound;
        widthUpperBound = (currentPiece.positions[i][0] > widthUpperBound) ? currentPiece.positions[i][0] : widthUpperBound;
    }

    // Loop through rows
    for (int i = heightLowerBound; i < heightUpperBound + 1; i++)
    {
        // Loop from left to right
        string word;
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            if (grid[i][j] != ' ')
            {
                word += grid[i][j];
                vector<int> position = { j, i };
                vector<vector<int>> wordVector = positions.at(foundWords.size());
                wordVector.push_back(position);
            }
            else if (word != "")
            {
                if (word.size() >= MIN_WORD_SIZE)
                    foundWords.push_back(word);
                word = "";
            }
        }
        // Loop from right to left
        word = "";
        for (int j = GRID_WIDTH - 1; j >= 0; j--)
        {
            if (grid[i][j] != ' ')
            {
                word += grid[i][j];
                int wordIndex = foundWords.size() - 1;
                vector<int> position = { wordIndex, j, i };
                vector<vector<int>> wordVector = positions.at(foundWords.size());
                wordVector.push_back(position);
            }
            else if (word != "")
            {
                if (word.size() >= MIN_WORD_SIZE)
                    foundWords.push_back(word);
                word = "";
            }
        }
    }

    // Loop through columns
    for (int i = widthLowerBound; i < widthUpperBound + 1; i++)
    {
        // Loop from top to bottom
        string word;
        for (int j = 0; j < GRID_HEIGHT; j++)
        {
            if (grid[j][i] != ' ')
            {
                word += grid[j][i];
                int wordIndex = foundWords.size() - 1;
                vector<int> position = { wordIndex, j, i };
                vector<vector<int>> wordVector = positions.at(foundWords.size());
                wordVector.push_back(position);
            }
            else if (word != "")
            {
                if (word.size() >= MIN_WORD_SIZE)
                    foundWords.push_back(word);
                word = "";
            }
        }
        // Loop from bottom to top
        word = "";
        for (int j = GRID_HEIGHT - 1; j >= 0; j--)
        {
            if (grid[j][i] != ' ')
            {
                word += grid[j][i];
                int wordIndex = foundWords.size() - 1;
                vector<int> position = { wordIndex, j, i };
                vector<vector<int>> wordVector = positions.at(foundWords.size());
                wordVector.push_back(position);
            }
            else if (word != "")
            {
                if (word.size() >= MIN_WORD_SIZE)
                    foundWords.push_back(word);
                word = "";
            }
        }
    }

    // Loop through validWords and words vector. If valid word is in word in vector, remove the letters from the grid using positions vector
    for (string validWord : validWords)
    {
        for (int i = 0; i < foundWords.size(); i++)
        {
            int index = foundWords[i].find(validWord);
            if (index != string::npos)
            {
                // Made a valid word, increase the score and level
                string reverseWord = validWord;
                reverse(reverseWord.begin(), reverseWord.end());
                int bonusMultiplier = (validWord == reverseWord) ? PALINDROME_MULTIPLIER : 1;
                currentScore += level * bonusMultiplier * validWord.size();
                wordsMade++;
                if (wordsMade > level * 5)
                {
                    level++;
                    speed *= 0.9f;
                }
                // Remove the valid word's characters on the grid
                for (int j = index; j < index + validWord.size(); j++)
                {
                    vector<int> characterPosition = positions.at(i).at(j);
                    grid[characterPosition.at(1)][characterPosition.at(0)] = ' ';
                }
            }
        }
    }
    // Looping through diagonals might be overpowered and make the game too easy
}