#include <iostream>
#include <fstream>
#include "Game.h"

using namespace std;

Game::Game() : isRunning(true), gameOver(false), window(nullptr), renderer(nullptr), font(nullptr), engine(nullptr), bgMusic(nullptr), currentScore(0), speed(START_SPEED), level(START_LEVEL), highScore(0), downPressed(false), lastDropTicks(0), instantDropped(0), currentPiece(generator, distribution), nextPiece(generator, distribution)
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

    // Attempt to open words_alpha.txt
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

    // Read valid words from file into validWords map
    string word;
    unordered_map<char, int> characterCounts;
    for (char character : ALPHABET)
    {
        characterCounts.insert({ character, 0 });
    }
    while (getline(wordsAlphaFile, word))
    {
        if (word.size() >= MIN_WORD_SIZE)
        {
            for (auto& character : word)
            {
                character = toupper(character); // Words have to be in upper case
                characterCounts.at(character)++;
            }
            validWords.insert(word);
        }
    }
    
    wordsAlphaFile.close();

    // Initialise letterWeights using characterCounts
    vector<int> letterWeights;
    for (auto pair : characterCounts)
    {
        letterWeights.push_back(pair.second);
    }

    // Initialise distribution using letterWeights
    std::discrete_distribution<int> tempDistribution(letterWeights.begin(), letterWeights.end());
    distribution = tempDistribution;

    // Initialise pieces using distribution
    Piece tempCurrent(generator, distribution);
    Piece tempNext(generator, distribution);
    currentPiece = tempCurrent;
    nextPiece = tempNext;

    // Initialise sound engine
    engine = irrklang::createIrrKlangDevice();

    if (!engine)
    {
        cerr << "Error: sound engine could not be created." << endl;
        isRunning = false;
        return;
    }

    // Start playing the background music
    bgMusic = engine->play2D("./audio/main-theme-rondeau.ogg", true, false, true);

    if (!bgMusic)
    {
        cerr << "Error: failed to play background music." << endl;
        isRunning = false;
        return;
    }

}

void Game::CleanUp()
{
    bgMusic->drop();
    engine->drop();
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
        SDL_Delay(floor(16.666f - msElapsed)); // Cap the game to 60FPS
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
        else if (!gameOver)
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
                instantDropped = true;
            }
            if (!downPressed && event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_DOWN) // Fast Drop
            {
                downPressed = true;
                speed /= FAST_DROP_MULTIPLIER;
                cout << "Down Pressed, speed increased to " << speed << endl;
            }
            if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_DOWN) // Reset speed after fast drop
            {
                downPressed = false;
                speed *= FAST_DROP_MULTIPLIER;
                cout << "Down Released, speed decreased to " << speed << endl;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_Z) // Need to only activate once
            {
                currentPiece.Rotate(grid, false);
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_X)
            {
                currentPiece.Rotate(grid);
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_C)
            {
                currentPiece.ShuffleLetters();
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE)
            {
                currentPiece.ChangeShape(grid);
            }
        }
    }
}

void Game::Update()
{
    if (!gameOver)
    {
        // If the current piece overlaps with anything, game over
        if (currentPiece.IsOverlapping(grid))
        {
            gameOver = true;
            bgMusic->stop();
            engine->play2D("./audio/game-over.ogg");
        }
        else
        {
            // Drop the current piece every so often, based on speed
            Uint64 currentTicks = SDL_GetTicks64();
            Uint64 msSinceDrop = currentTicks - lastDropTicks;

            // If the current piece attempts to drop but is blocked, fix it in place
            if (msSinceDrop > speed || instantDropped) // If ms since last drop is greater than speed
            {
                instantDropped = false;
                bool didDrop = currentPiece.Drop(grid);
                lastDropTicks = SDL_GetTicks64();
                if (!didDrop)
                {
                    currentPiece.Fix(grid);

                    // Check for words and increase score/level/speed if necessary
                    CheckWords();

                    // Change the current and next pieces
                    currentPiece = nextPiece;

                    Piece newPiece(generator, distribution);

                    nextPiece = newPiece;

                    highScore = (currentScore > highScore) ? currentScore : highScore;
                }
            }
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
        SDL_RenderDrawLine(renderer, xPosition, 0, xPosition, CELL_LENGTH * GRID_HEIGHT);
    }
    for (int i = 0; i <= GRID_HEIGHT; i++) // Horizontal lines
    {
        float yPosition = i * CELL_LENGTH;
        SDL_RenderDrawLine(renderer, 0, yPosition, CELL_LENGTH * GRID_WIDTH, yPosition);
    }

    // Render characters in grid
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Color textColor = { 190, 193, 190, 255 };
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        for (int j = 0; j < GRID_WIDTH; j++)
        {
            char gridValue = grid[i][j];
            if (gridValue != ' ')
            {
                if (VOWELS.find(gridValue) != string::npos)
                {
                    textColor = { 241, 240, 170, 255 };
                }
                else
                {
                    textColor = { 190, 193, 190, 255 };
                }
                if (gameOver)
                {
                    textColor = { 255, 0, 0, 255 };
                }
                int textX = (j * CELL_LENGTH) + 2 + ((CELL_LENGTH - FONT_SIZE) / 2);
                int textY = (i * CELL_LENGTH) + ((CELL_LENGTH - FONT_SIZE) / 2);

                SDL_Surface* surface = TTF_RenderGlyph_Solid(font, gridValue, textColor); // Might need to change text colour
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
        int textX = (currentPiece.positions[i][0] * CELL_LENGTH) + 2 + ((CELL_LENGTH - FONT_SIZE) / 2);
        int textY = (currentPiece.positions[i][1] * CELL_LENGTH) + ((CELL_LENGTH - FONT_SIZE) / 2);

        SDL_Surface* surface = TTF_RenderGlyph_Solid(font, character, { 255, 255, 255, 255 }); // Might need to change text colour
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect textRect{ textX, textY, FONT_SIZE, FONT_SIZE };
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_DestroyTexture(texture);
    }

    // Render nextPiece at top right
    for (int i = 0; i < 3; i++)
    {
        char character = nextPiece.GetCharacter(i);
        int gridRightSide = GRID_WIDTH * CELL_LENGTH;
        int midPoint = gridRightSide + (WINDOW_WIDTH - gridRightSide) / 2 - CELL_LENGTH * 1.5;
        int textX = midPoint + (nextPiece.positions[i][0] - 4) * CELL_LENGTH + ((CELL_LENGTH - FONT_SIZE) / 2); // -4 is a magic number here based on grid spawn point positions in Piece constructor
        int textY = (nextPiece.positions[i][1] * CELL_LENGTH) + ((CELL_LENGTH - FONT_SIZE) / 2);

        SDL_Surface* surface = TTF_RenderGlyph_Solid(font, character, { 255, 255, 255, 255 }); // Might need to change text colour
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect textRect{ textX, textY, FONT_SIZE, FONT_SIZE };
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_DestroyTexture(texture);
    }

    // Render high score 
    int gridRightSide = GRID_WIDTH * CELL_LENGTH;
    int midPoint = gridRightSide + (WINDOW_WIDTH - gridRightSide) / 2;
    string text = "High Score: " + to_string(highScore);
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), {255, 255, 255, 255}); // Might need to change text colour
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int textX = midPoint - (surface->w / 2);
    int textY = 100;
    SDL_Rect textRect{ textX, textY, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Render current score
    text = "Current Score: " + to_string(currentScore);
    surface = TTF_RenderText_Solid(font, text.c_str(), { 255, 255, 255, 255 }); // Might need to change text colour
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    textX = midPoint - (surface->w / 2);
    textY = 150;
    textRect = { textX, textY, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Render level
    text = "Level: " + to_string(level);
    surface = TTF_RenderText_Solid(font, text.c_str(), { 255, 255, 255, 255 }); // Might need to change text colour
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    textX = midPoint - (surface->w / 2);
    textY = 200;
    textRect = { textX, textY, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Render number of wordsMade
    text = "Words Made: " + to_string(wordsMade.size());
    surface = TTF_RenderText_Solid(font, text.c_str(), { 255, 255, 255, 255 }); // Might need to change text colour
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    textX = midPoint - (surface->w / 2);
    textY = 250;
    textRect = { textX, textY, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Render last 3 words of wordsMade
    for (int i = 0; i < 3 && i < wordsMade.size(); i++)
    {
        text = wordsMade.at(wordsMade.size() - 1 - i);
        surface = TTF_RenderText_Solid(font, text.c_str(), { 255, 255, 255, 255 });
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        textX = midPoint - (surface->w / 2);
        textY = 300 + (i * FONT_SIZE);
        textRect = { textX, textY, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    // Render game over text if game is over
    if (gameOver)
    {
        text = "Game Over!";
        surface = TTF_RenderText_Solid(font, text.c_str(), { 255, 0, 0, 255 });
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        textX = midPoint - (surface->w / 2);
        textY = WINDOW_HEIGHT - FONT_SIZE;
        textRect = { textX, textY, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer);
}

void Game::CheckWords()
{
    // Only need to check rows/columns concerned with currentPiece, no need to go through the whole board
    int heightLowerBound = 999;
    int widthLowerBound = 999;
    for (int i = 0; i < 3; i++)
    {
        heightLowerBound = (currentPiece.positions[i][1] < heightLowerBound) ? currentPiece.positions[i][1] : heightLowerBound; // The "lowest" (visually highest) character's y position
        widthLowerBound = (currentPiece.positions[i][0] < widthLowerBound) ? currentPiece.positions[i][0] : widthLowerBound; // The left-most character's x position
    }
    int heightUpperBound = -1;
    int widthUpperBound = -1;
    for (int i = 0; i < 3; i++)
    {
        heightUpperBound = (currentPiece.positions[i][1] > heightUpperBound) ? currentPiece.positions[i][1] : heightUpperBound; // The "highest" (visually lowest) character's y position
        widthUpperBound = (currentPiece.positions[i][0] > widthUpperBound) ? currentPiece.positions[i][0] : widthUpperBound; // The right-most character's x position
    }

    // Loop through rows
    for (int i = heightLowerBound; i <= heightUpperBound; i++)
    {
        // Loop from left to right
        for (int j = 0; j <= GRID_WIDTH - MIN_WORD_SIZE; j++)
        {
            // Loop backwards through the row to check for bigger words first
            for (int k = GRID_WIDTH - 1; k >= j + MIN_WORD_SIZE - 1; k--) 
            {
                string potentialWord(grid[i], j, k - j + 1);
                bool wordIsValid = ValidateWord(potentialWord);
                if (wordIsValid)
                {
                    UpdateScore(potentialWord);
                    // Remove the word from the grid
                    for (int l = j; l <= k; l++)
                    {
                        grid[i][l] = ' ';
                    }
                }
            }
        }
        // Loop from right to left
        for (int j = GRID_WIDTH - 1; j >= MIN_WORD_SIZE - 1; j--)
        {
            // Loop frontwards through the row to check for bigger words first
            for (int k = 0; k <= j - MIN_WORD_SIZE + 1; k++)
            {
                string potentialWord(grid[i], k, j - k + 1);
                reverse(potentialWord.begin(), potentialWord.end()); // Word needs to be reversed because we want to read the word backwards
                bool wordIsValid = ValidateWord(potentialWord);
                if (wordIsValid)
                {
                    UpdateScore(potentialWord);
                    // Remove the word from the grid
                    for (int l = k; l <= j; l++)
                    {
                        grid[i][l] = ' ';
                    }
                }
            }
        }
    }

    // Loop through columns
    for (int i = widthLowerBound; i <= widthUpperBound; i++)
    {
        // Loop from top to bottom
        string columnOfChars;
        for (int j = 0; j < GRID_HEIGHT; j++)
        {
            columnOfChars += grid[j][i];
        }
        // Loop through the columnOfChars string
        for (int j = 0; j <= columnOfChars.size() - MIN_WORD_SIZE; j++)
        {
            // Loop backwards through the column to check for bigger words first
            for (int k = columnOfChars.size() - 1; k >= j + MIN_WORD_SIZE - 1; k--)
            {
                string potentialWord(columnOfChars, j, k - j + 1);
                bool wordIsValid = ValidateWord(potentialWord);
                if (wordIsValid)
                {
                    UpdateScore(potentialWord);
                    // Remove the word from the grid
                    for (int l = j; l <= k; l++)
                    {
                        grid[l][i] = ' ';
                    }
                }
            }
        }

        // Loop from bottom to top
        columnOfChars = "";
        for (int j = GRID_HEIGHT - 1; j >= 0; j--)
        {
            columnOfChars += grid[j][i];
        }
        // Loop through the columnOfChars string
        for (int j = 0; j <= columnOfChars.size() - MIN_WORD_SIZE; j++)
        {
            // Loop through the column to check for bigger words first
            for (int k = columnOfChars.size() - 1; k >= j + MIN_WORD_SIZE - 1; k--)
            {
                string potentialWord(columnOfChars, j, k - j + 1);
                bool wordIsValid = ValidateWord(potentialWord);
                if (wordIsValid)
                {
                    UpdateScore(potentialWord);
                    // Remove the word from the grid
                    for (int l = j; l <= k; l++)
                    {
                        grid[GRID_HEIGHT - 1 - l][i] = ' ';
                    }
                }
            }
        }
    }

    // Loop through all columns to drop letters down using "gravity"
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        for (int j = GRID_HEIGHT - 1; j >= 0; j--)
        {
            if (grid[j][i] != ' ' && grid[j + 1][i] == ' ')
            {
                int k = j;
                while (k < GRID_HEIGHT - 1 && grid[k + 1][i] == ' ')
                {
                    k++;
                }
                grid[k][i] = grid[j][i];
                grid[j][i] = ' ';
            }
        }
    }

    // Need to recursively check for words again if letters dropped from gravity, but this would cause very sudden chains/combos. Need "animations"
}

bool Game::ValidateWord(const string word)
{
    if (validWords.find(word) != validWords.end())
    {
        //cout << "Valid word found: " << word << endl;
        return true;
    }
    else
    {
        return false;
    }
}

void Game::UpdateScore(const string word)
{
    // Made a valid word, increase the score and level
    wordsMade.push_back(word);
    string reverseWord = word;
    reverse(reverseWord.begin(), reverseWord.end());
    int bonusMultiplier = 1;
    if (word == reverseWord)
    {
        bonusMultiplier = PALINDROME_MULTIPLIER;
    }
    currentScore += level * word.size() * bonusMultiplier;
    if (wordsMade.size() > level * 5)
    {
        level++;
        speed *= 0.9f;
    }
}