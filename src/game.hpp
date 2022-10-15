#pragma once

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include "tetrisBlock.hpp"

#define YELLOW 1
#define CYAN 2
#define BLUE 3
#define WHITE 4
#define GREEN 5
#define RED 6
#define MAGENTA 7
#define BLACK 8
#define WHITE_TEXT 9
#define YELLOW_TEXT 10
#define RED_TEXT 11

#define DELAY 1000
#define BLOCK_WIDTH 2

class Game 
{
public:
    void init();
    void gameloop();
    void exit();
private:
    WINDOW* gameBoard;
    WINDOW* upNext;
    WINDOW* instructions;
    WINDOW* scores;
    WINDOW* gameOver;

    Block current;
    Block next;

    int score;
    int highscore;

    int boardWidth;
    int boardHeight;
    int board[20][10];
    int preview[4][4];

    int updateCount;
    int updateRate;

    int level;
    int linesCleared;

    int key;

    void clearLines();
    void leveling();

    void writeHighScore();
    int readHighScore();
    void increaseDifficulty();

    void initColors();
    void printColor(WINDOW* w, int x, int y, int color, const char* str);

    void renderBoard();
    void renderPreview();;
    void renderInstructions();
    void renderScores();

    void reset();
};
