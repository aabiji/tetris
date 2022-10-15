#pragma once

#include <time.h>
#include <stdlib.h>
#include <vector>

class Block 
{
public:
    Block();
    bool gameOver(int (&board)[20][10]);
    int hardDrop(int (&board)[20][10], int distance);
    void moveLeft(int (&board)[20][10]);
    void moveRight(int (&board)[20][10]);
    bool moveDown(int (&board)[20][10]);
    void rotateLeft(int (&board)[20][10]);
    void rotateRight(int (&board)[20][10]);
    int (*returnRotation(int direction))[2];
    void addToBoard(int (&board)[20][10]);
    void removeFromBoard(int (&board)[20][10]);
    void addToPreview(int (&upNext)[4][4]);
    bool canMove(int (&board)[20][10], int block[4][2], int x, int y);
private:
    int x;
    int y;
    int type;
    int rotateIndex;
    int block[4][2];
    bool inBounds(int x, int y, int block[4][2]);
};
