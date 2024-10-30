# Letrio Report
Letrio is a falling-block puzzle game about making words.

## Dependencies
- SDL2 library - https://www.libsdl.org/
- SDL2_ttf library - https://github.com/libsdl-org/SDL_ttf
- irrKlang library - https://www.ambiera.com/irrklang/downloads.html

## Third-party Assets
- KarenFat.ttf font - https://www.pentacom.jp/pentacom/bitfontmaker2/gallery/?id=346
- words_alpha.txt file - https://github.com/dwyl/english-words

## YouTube Video
YouTube Video: [insert youtube video]

## Game Objective
The objective of Letrio is to use letters within given tromino pieces to make words in a grid. A piece will fall from the top of the grid, and when it reaches the bottom, it will fix into place, and the next piece will be generated at the top. The player can manipulate pieces to place letters in different positions to create words. When a word is created, the player's score will increase, the letters within the word will disappear from the grid, and gravity will cause any letters within the affected columns to drop.

## Controls
- Left/Right Arrow Key - Move piece left/right
- Down Arrow Key (Hold) - Drop piece faster
- Up Arrow Key - Drop piece instantly
- Z/X - Rotate piece anticlockwise/clockwise
- C - Shuffle letters in piece
- Space - Change piece shape
- Return - Pause game
- Escape - Quit game

## Game Programming Patterns
- Game Loop
- Update Method

## Game Mechanics + Example Code

### Moving Pieces
Pieces can move left and right. They can also drop faster or instantaneously.
### Rotating Pieces
Pieces can rotate clockwise or anticlockwise. The origin of rotation is the grid cell of the second/middle character.
### Shuffling Letters (Unique)
Letters within a piece can shuffle positions.
### Changing Piece Shape (Unique)
Pieces can change between an L shape and a Line shape.
### Making Words
Valid words are four or more letters in length. Words must be written from left to right, or from top to bottom.
### Gravity
When pieces are fixed to the board, gravity takes effect and drops letters as low as possible. This can create combos of making words.
### Levels
For every 5 words the player makes, the level will increase. This will cause the pieces to drop faster and increase the difficulty.
### Pause Anticheat
To prevent players from pausing to cheat and slow the game down, the screen will clear the grid while the game is paused.

## Design Diagrams
Insert diagram about orientations and shapes. Maybe a class/object diagram too with Game and Piece.

## Sample Screens
Insert screens for start, normal gameplay, pause and game over.

## Exception handling + Test Cases
Insert game initialisation stuff from constructor. Maybe mention conditional checks in movement code.

## Evaluation
I believe that I have created a fun prototype of a game that demonstrates a good understand of writing code in the object-oriented paradigm. I am particularly proud of the audio feedback that the game provides, which significantly improves the immersion of the game in my opinion. If I were to continue working on this project, I would try to focus on visual feedback, as many actions in the game feel instantaneous, particularly when using gravity to create combos.