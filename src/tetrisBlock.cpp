#include "tetrisBlock.hpp"

int blocks[7][4][2] = { {{0, 0}, {1, 0}, {0, 1}, {1, 1}},   // O
                        {{0, 0}, {-1, 0}, {1, 0}, {2, 0}},  // I
                        {{0, 0}, {-1, 0}, {-1, 1}, {1, 0}}, // J
                        {{0, 0}, {-1, 0}, {1, 0}, {1, 1}},  // L
                        {{0, 0}, {-1, 0}, {0, 1}, {1, 1}},  // S
                        {{0, 0}, {-1, 1}, {1, 0}, {0, 1}},  // Z
                        {{0, 0}, {-1, 0}, {1, 0}, {0, 1}}   // T 
                      };
std::vector<int> choices = {0, 1, 2, 3, 4, 5, 6};

Block::Block()
{
   srand(time(NULL));
    
   if (choices.size() == 0) choices = {0, 1, 2, 3, 4, 5, 6};
   int choice = rand() % choices.size();
   type = choices[choice];
   choices.erase(choices.begin() + choice);

   int min = 1;
   int max = 7;
   x = rand() % (max - min + 1) + min;
   y = 1;

   for (int i = 0; i < 4; i++) {
       block[i][0] = blocks[type][i][0];
       block[i][1] = blocks[type][i][1];
   }
}

bool Block::gameOver(int (&board)[20][10])
{
    if (!canMove(board, block, x, y + 1) and y == 1) return true;
    return false;
}

void Block::addToBoard(int (&board)[20][10])
{
    for (int i = 0; i < 4; i++) {
        board[y - block[i][1]][x + block[i][0]] = type + 1;
    }
}

void Block::removeFromBoard(int (&board)[20][10])
{
    for (int i = 0; i < 4; i++) {
        board[y - block[i][1]][x + block[i][0]] = 0;
    }
}

void Block::addToPreview(int (&upNext)[4][4])
{
    for (int i = 0; i < 4; i++) {
        upNext[2 - block[i][1]][1 + block[i][0]] = type + 1;
    }
}

bool Block::inBounds(int x, int y, int block[4][2])
{
    bool flag = true;
    for (int i = 0; i < 4; i++) {
        if (x + block[i][0] > 9 || x + block[i][0] < 0) {
            flag = false;
            break;
        }
        if (y - block[i][1] > 19) {
            flag = false;
            break;
        }
    }
    return flag;
}

bool Block::canMove(int (&board)[20][10], int block[4][2], int x, int y)
{
    bool collision = false;
    
    removeFromBoard(board);

    for (int i = 0; i < 4; i++) {
        int type = board[y - block[i][1]][x + block[i][0]];
        if (type != 0) {
            collision = true;
            break;
        }
    }

    return !collision;
}

int Block::hardDrop(int (&board)[20][10], int distance)
{
    if (moveDown(board)) {
        distance ++;
        return hardDrop(board, distance);
    }
    return distance;
}

void Block::moveLeft(int (&board)[20][10])
{
    if (inBounds(x - 1, y, block) && canMove(board, block, x - 1, y)) {
        removeFromBoard(board);
        x--;
    }
}

void Block::moveRight(int (&board)[20][10])
{
    if (inBounds(x + 1, y, block) && canMove(board, block, x + 1, y)) {
        removeFromBoard(board);
        x++;
    }
}

bool Block::moveDown(int (&board)[20][10])
{
    if (inBounds(x, y + 1, block) && canMove(board, block, x, y + 1)) {
        removeFromBoard(board);
        y++;
        return true;
    } else {
        return false;
    }

}

int (*Block::returnRotation(int direction))[2]
{
    int (*b)[2] = new int[4][2];

    for (int i = 0; i < 4; i++) {
        b[i][0] = block[i][0];
        b[i][1] = block[i][1];
    }

    for (int i = 0; i < 4; i++) {
        int x = b[i][0];
        int y = b[i][1];

        if (direction == -1) {
            b[i][0] = -y;
            b[i][1] = x;
        } else {
            b[i][1] = -x;
            b[i][0] = y;
        }
    }

    return b;
}

void Block::rotateLeft(int (&board)[20][10])
{
    removeFromBoard(board);
    if (type == 0) return;
    
    if (inBounds(x, y, returnRotation(-1)) && canMove(board, returnRotation(-1), x, y)) {
        for (int i = 0; i < 4; i++) {
            int x = block[i][0];
            int y = block[i][1];
            block[i][0] = -y;
            block[i][1] = x;
        }
    }
}

void Block::rotateRight(int (&board)[20][10])
{
    removeFromBoard(board);
    if (type == 0) return;

    if (inBounds(x, y, returnRotation(1)) && canMove(board, returnRotation(1), x, y)) {
        for (int i = 0; i < 4; i++) {
            int x = block[i][0];
            int y = block[i][1];
            block[i][1] = -x;
            block[i][0] = y;
        }
    }
}
