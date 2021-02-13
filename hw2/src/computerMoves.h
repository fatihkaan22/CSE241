#ifndef COMPUTER_MOVES_H
#define COMPUTER_MOVES_H

#include <iostream>
#include <random>

#include "stack.h"
#include "gameStructures.h"

using namespace std;

enum STATUS {
    defense, attack
};

typedef struct memory {
    stack opponentPos;
    STATUS status = attack;
    pos computerLastMove;
    pos nextMove;
} memory;

// it will either choose to attack or defense randomly
// defense: tries to block possible bridge of opponent
// attack: tries to connect one side to other side
void smartMove(CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize, memory &computerMemory);

int random(const int &end, const int &start = 0);

// returns an integer between start and end
int random(const int &end, const int &start) {
    random_device randomDevice;
    unsigned seed = randomDevice();
    default_random_engine rd(seed);
    return start + rd() % end;
}

void initMemory(memory &computerMemory, const int &boardSize) {
    computerMemory.nextMove.x = boardSize / 2;
    computerMemory.nextMove.y = 0;
    computerMemory.computerLastMove.x = 0;
    computerMemory.computerLastMove.y = 0;
}

// prints x y so that the user will understand
void printPos(const int &x, const int &y) {
    char c = 'a' + x;
    cout << c << y + 1 << endl;
}

// push last opponent's move to stack
void getOpponentPos(stack &stack, const CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[j][i] == X_CELL) {
                pos p;
                p.x = j;
                p.y = i;
                if (!exist(stack, p)) {
                    push(stack, p);
                    break;
                }
            }
        }
    }
}

pos getLastMove(const stack &opponentsPos) {
    return peek(opponentsPos);
}

// every position has 6 bridges unless those are not out of the map
// however this will consider only 1 which is closer the opponents border
// x, (y+2): same
// x, (y-2): same
// (x+2), (y-1): right
// (x+1), (y+1): right
// (x-2), (y+1): left
// (x-1), (y-1): left
bool blockPossibleBridge(CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize,
                         const pos &lastMove, pos &computerLastMove) {
    char border = (lastMove.x < boardSize / 2) ? 'l' : 'r'; // left or right (closest)
    bool decided = false;
    pos decision;
    if (border == 'l') {
        if (isAvailable(lastMove.x - 2, lastMove.y + 1, board, boardSize)) {
            decision.x = lastMove.x - 2;
            decision.y = lastMove.y + 1;
            decided = true;
        } else if (isAvailable(lastMove.x - 1, lastMove.y - 1, board, boardSize)) {
            decision.x = lastMove.x - 1;
            decision.y = lastMove.y - 1;
            decided = true;
        }
    } else if (border == 'r') {
        if (isAvailable(lastMove.x + 2, lastMove.y - 1, board, boardSize)) {
            decision.x = lastMove.x + 2;
            decision.y = lastMove.y - 1;
            decided = true;
        } else if (isAvailable(lastMove.x + 1, lastMove.y + 1, board, boardSize)) {
            decision.x = lastMove.x + 1;
            decision.y = lastMove.y + 1;
            decided = true;
        }
    }
    if (decided) {
        board[decision.x][decision.y] = O_CELL;
        printPos(decision.x, decision.y);
        computerLastMove.x = decision.x;
        computerLastMove.y = decision.y;
    }
    return decided;
}


bool blockOpponentsWay(CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize, memory &computerMemory) {
    getOpponentPos(computerMemory.opponentPos, board, boardSize);
    pos lastMove = getLastMove(computerMemory.opponentPos);
    return blockPossibleBridge(board, boardSize, lastMove, computerMemory.computerLastMove);
}

// tries to connect one side to other
bool tryConnection(CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize, memory &computerMemory) {
    bool played = false;
    int x = -1, y;
    pos nextMove = computerMemory.nextMove;
    if (board[nextMove.x][nextMove.y] == O_CELL || board[nextMove.x][nextMove.y] == EMPTY_CELL) {
        if (isAvailable(nextMove.x, nextMove.y, board, boardSize)) {
            x = nextMove.x;
            y = nextMove.y;
        } else if (isAvailable(nextMove.x, nextMove.y + 1, board, boardSize)) {
            x = nextMove.x;
            y = nextMove.y + 1;
        } else if (isAvailable(nextMove.x - 1, nextMove.y + 1, board, boardSize)) {
            x = nextMove.x - 1;
            y = nextMove.y + 1;
        }
    } else {
        if (isAvailable(nextMove.x + 1, nextMove.y, board, boardSize)) {
            x = nextMove.x + 1;
            y = nextMove.y;
        } else if (isAvailable(nextMove.x - 1, nextMove.y, board, boardSize)) {
            x = nextMove.x - 1;
            y = nextMove.y;
        }
    }
    if (x != -1) {
        board[x][y] = O_CELL;
        printPos(x, y);
        played = true;
        computerMemory.nextMove.x = x;
        computerMemory.nextMove.y = y;
    }
    return played;
}

// tries to fill neighbors of last move (not very smart but still better than random)
bool tryNeighbors(pos &computerLastMove, CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize) {
    pos neighbors[6];
    fillNeighbors(neighbors, computerLastMove.x, computerLastMove.y);
    for (auto & neighbor : neighbors)
        if (isAvailable(neighbor.x, neighbor.y, board, boardSize)) {
            board[neighbor.x][neighbor.y] = O_CELL;
            printPos(neighbor.x, neighbor.y);
            computerLastMove.x = neighbor.x;
            computerLastMove.y = neighbor.y;
            return true;
        }
    return false;
}

// finds all free positions in the board, and randomly puts the mark one of these positions
void randomMove(CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize) {
    pos arr[MAX_SIZE * MAX_SIZE];
    int index = 0;
    for (int i = 0; i < boardSize; ++i)
        for (int j = 0; i < boardSize; ++i)
            if (board[j][i] == EMPTY_CELL) {
                pos p;
                p.x = j;
                p.y = i;
                arr[index++] = p;
            }

    int r = random(index - 1);
    board[arr[r].x][arr[r].y] = O_CELL;
    printPos(arr[r].x, arr[r].y);
}

void smartMove(CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize, memory &computerMemory) {
    bool played = false;

    switch (random(2)) {
        case 0:
            computerMemory.status = STATUS::defense;
            break;
        case 1:
            computerMemory.status = STATUS::attack;
            break;
    }

    if (computerMemory.status == STATUS::defense)
        played = blockOpponentsWay(board, boardSize, computerMemory);
    if (!played && computerMemory.status == STATUS::attack)
        played = tryConnection(board, boardSize, computerMemory);
    if (!played)
        played = tryNeighbors(computerMemory.computerLastMove, board, boardSize);
    if (!played)
        randomMove(board, boardSize);
}

#endif