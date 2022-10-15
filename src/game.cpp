#include "game.hpp"

void Game::initColors()
{
    start_color();
    init_pair(RED, COLOR_RED, COLOR_RED);
    init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(CYAN, COLOR_CYAN, COLOR_CYAN);
    init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
    init_pair(BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(WHITE_TEXT, COLOR_WHITE, COLOR_BLACK);
    init_pair(YELLOW_TEXT, COLOR_YELLOW, COLOR_BLACK);
    init_pair(RED_TEXT, COLOR_RED, COLOR_BLACK);
}

int Game::readHighScore()
{
    int score = 0;
    std::ifstream  f("save.dat", std::ios::in | std::ios::binary);
    f >> score;
    f.close();
    return score;
}

void Game::writeHighScore()
{
    if (score > highscore) {
        std::ofstream f("save.dat", std::ios::out | std::ios::binary);
        f << std::to_string(score);
        f.close();
    }
}

void Game::init()
{
    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
    initColors();
    clear();

    upNext = newwin(6, 10, 1, 24);
    instructions = newwin(15, 35, 8, 24);
    scores = newwin(6, 24, 1, 35);

    boardWidth = 10;
    boardHeight = 20;
    gameBoard = newwin(boardHeight + 2, (boardWidth * BLOCK_WIDTH) + 2,  1, 1);
    
    current = Block();
    next = Block();

    updateRate = 110;

    score = 0;
    highscore = readHighScore();

    level = 0;
    linesCleared = 0;

    key = 0;
}

void Game::printColor(WINDOW* w, int x, int y, int color, const char* str)
{
    wattron(w, COLOR_PAIR(color));
    mvwaddstr(w, y, x, str);
    wattroff(w, COLOR_PAIR(color));
}

void Game::renderBoard()
{
    werase(gameBoard);
    box(gameBoard, 0, 0);
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            int block = board[y][x];
            if (block == 0) continue;
            printColor(gameBoard, (x * BLOCK_WIDTH) + 1, y + 1, block, "  ");
        }
    }
    wrefresh(gameBoard);
}

void Game::renderPreview()
{
    werase(upNext);
    box(upNext, 0, 0);
    printColor(upNext, 1, 0, WHITE_TEXT, "Up next:");
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            printColor(upNext, (x * BLOCK_WIDTH) + 1, y + 1, preview[y][x], "  ");
        }
    }
    wrefresh(upNext);
    memset(preview, 0, sizeof(preview));
}

void Game::renderInstructions()
{
    box(instructions, 0, 0);
    printColor(instructions, 12, 0, WHITE_TEXT, "Controls:");
    printColor(instructions, 2, 1, WHITE_TEXT, "Left arrow key : move left");
    printColor(instructions, 2, 3, WHITE_TEXT, "Right arrow key : move right");
    printColor(instructions, 2, 5, WHITE_TEXT, "Up arrow key or x: rotate right");
    printColor(instructions, 2, 7, WHITE_TEXT, "z : rotate left");
    printColor(instructions, 2, 9, WHITE_TEXT, "Space: Hard drop");
    printColor(instructions, 2, 11, WHITE_TEXT, "Down arrow key: soft drop");
    printColor(instructions, 2, 13, WHITE_TEXT, "q : quit");
    wrefresh(instructions);
}

void Game::renderScores()
{
    werase(scores);
    box(scores, 0, 0);
    printColor(scores, 9, 0, WHITE_TEXT, "Stats:");
    printColor(scores, 7, 1, WHITE_TEXT, "Score:");
    printColor(scores, 15, 1, YELLOW_TEXT, std::to_string(score).c_str());
    printColor(scores, 4, 2, WHITE_TEXT, "High score: ");
    printColor(scores, 17, 2, YELLOW_TEXT, std::to_string(highscore).c_str());
    printColor(scores, 7, 3, WHITE_TEXT, "Level:");
    printColor(scores, 15, 3, YELLOW_TEXT, std::to_string(level).c_str());
    printColor(scores, 3, 4, WHITE_TEXT, "Lines cleared: ");
    printColor(scores, 18, 4, YELLOW_TEXT, std::to_string(linesCleared).c_str());
    wrefresh(scores);
}

void Game::clearLines() 
{
    int lines = 0;
    for (int line = 19; line > 0; line--) {
        int fullCount = 0;
        for (int x = 0; x < 10; x++) {
            if (board[line][x] != 0) fullCount++;
        }
        if (fullCount == 0) break;
        if (fullCount == 10) {
            lines++;
            for (int y = line; y > 0; y--)
                memcpy(board[y], board[y - 1], sizeof(board[y]));
        }
    }
    if (lines > 0 && !current.moveDown(board)) {
        current = next;
        next = Block();
        for (int i = 0; i < 4; i++) memset(preview[i], 0, sizeof(preview[i]));
        for (int i = 0; i < 3; i++) memset(board[i], 0, sizeof(board[i]));
    }
    linesCleared += lines;
}

void Game::reset()
{
    writeHighScore();
    score = 0;
    updateCount = 0;
    level = 0;
    linesCleared = 0;
    updateRate = 110;
    current = Block();
    next = Block();
    memset(board, 0, sizeof(board));
    memset(preview, 0, sizeof(preview));
    clear();

}

void Game::leveling()
{
    int levels[28] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 100, 100, 100, 100, 100, 110, 120, 130,
                     140, 150, 160, 170, 180, 190, 200, 200, 200, 200};
    if ((linesCleared - levels[level]) == 0) {
        updateCount = 0;
        updateRate -= 3;
        level ++;
    }
}

void Game::gameloop()
{
   do {
        if (current.gameOver(board)) reset();
        clearLines();
        leveling();

        updateCount++;
        if (updateCount % updateRate == 0) {
            bool canMove = current.moveDown(board);
            updateCount = 0;

            if (!canMove) {
                current.addToBoard(board);
                current = next;
                next = Block();

                for (int i = 0; i < 4; i++)
                    memset(preview[i], 0, sizeof(preview[i]));
            }
        }

        next.addToPreview(preview);
        current.addToBoard(board);

        renderBoard();
        renderPreview();
        renderInstructions();
        renderScores();
        
        key = getch();
        switch (key) {
            case KEY_LEFT:
                current.moveLeft(board);
                break;
            case KEY_RIGHT:
                current.moveRight(board);
                break;
            case KEY_DOWN:
                current.moveDown(board);
                score ++;
                break;
            // SPACE
            case 32:
                score += current.hardDrop(board, 4) * 2;
                break;
            // X
            case 88:
            case 120:
            case KEY_UP:
                current.rotateRight(board);
                break;
            // Z
            case 90:
            case 122:
                current.rotateLeft(board);
                break;
        }
       
        refresh();
        
        usleep(DELAY);
   } while (key != 113 && key != 81);
}

void Game::exit()
{
    writeHighScore();
    delwin(gameBoard);
    delwin(upNext);
    delwin(instructions);
    delwin(scores);
    endwin();
}
