// some common basic structures and functions

#ifndef GAME_STRUCTURES
#define GAME_STRUCTURES

typedef struct position {
    int x;
    int y;
} pos;

enum MARK {
    X = 'x', O = 'o'
};

enum CELL {
    X_CELL = 'x', O_CELL = 'o', EMPTY_CELL = '.', X_CELL_CAPITAL = 'X', O_CELL_CAPITAL = 'O'
};


const int MIN_SIZE = 6;
const int MAX_SIZE = 12;

// checks if given position inside of board
bool checkInsideOfBoard(const pos &p, const int &boardSize) {
    return p.x >= 0 && p.x <= boardSize
           && p.y >= 0 && p.y <= boardSize;
}

// checks if given position is haven't marked before and inside of the board
bool isAvailable(const int &x, const int &y, const CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize) {
    pos p;
    p.x = x;
    p.y = y;
    return checkInsideOfBoard(p, boardSize) && board[x][y] == EMPTY_CELL;
}

// fills arr according to given x, y values.
// at the end arr will include all the cells that neighbours of cell of x,y coordinates.
void fillNeighbors(pos arr[6], const int &x, const int &y) {
    arr[0].x = x;
    arr[0].y = y - 1;
    arr[1].x = x;
    arr[1].y = y + 1;
    arr[2].x = x - 1;
    arr[2].y = y;
    arr[3].x = x + 1;
    arr[3].y = y;
    arr[4].x = x - 1;
    arr[4].y = y + 1;
    arr[5].x = x + 1;
    arr[5].y = y - 1;
}

#endif