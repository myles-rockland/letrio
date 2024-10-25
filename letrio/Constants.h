#pragma once
#include <string>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 18;
const int CELL_LENGTH = WINDOW_HEIGHT / GRID_HEIGHT;
const int FONT_SIZE = CELL_LENGTH;
const int START_SPEED = 1000; // Inversely proportional to level
const int START_LEVEL = 1; // Might need to be 0.
const int MIN_WORD_SIZE = 4;
const int FAST_DROP_MULTIPLIER = 5;
const int PALINDROME_MULTIPLIER = 2;
const std::string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string VOWELS = "AEIOU";
const std::string WORDS_FILE_PATH = "words_alpha.txt";