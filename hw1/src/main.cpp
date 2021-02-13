#include <iostream>
#include <string>

#include "convert.h"
#include "computerMoves.h"
#include "gameStructures.h"

#define RED "\u001b[31m"
#define CYAN "\u001b[36m"
#define YELLOW "\u001b[33m"
#define RESET "\u001b[0m"

using namespace std;

// simple function that reads integer from cin between given boundaries
int inputInteger(int lowerBoundary, int upperBoundary);

bool isVsComputer();

void printBoard(char board[MAX_SIZE][MAX_SIZE], int boardSize);

// creates a board with given size, and fills with . character
void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int boardSize);

// marks either x or o according to markType, if given position is successfully marked returns true, false otherwise
bool mark(char board[MAX_SIZE][MAX_SIZE], int boardSize, string pos, MARK markType);

// checks if given position connected to the other side of the board
bool isConnected(pos p, int boardSize, char board[MAX_SIZE][MAX_SIZE], bool visited[MAX_SIZE][MAX_SIZE],
                 MARK markType, stack *path);

// sets false all the elements in arr
void initializeVisited(bool arr[MAX_SIZE][MAX_SIZE], int size);

// checks if one of the users finished the game, returns true if the case
bool checkFinished(char board[MAX_SIZE][MAX_SIZE], int boardSize, MARK markType);

// checks either one of the users is finished the game or the board is full
bool isBoardFull(char board[MAX_SIZE][MAX_SIZE], int boardSize);

bool isGameFinished(char board[MAX_SIZE][MAX_SIZE], int boardSize, char *winner);

void capitalizePath(char board[MAX_SIZE][MAX_SIZE], stack path, MARK mark);

// gets position from cin with some validations
void inputPos(string *pos);

int main() {
    cout << "Board size: ";
    char board[MAX_SIZE][MAX_SIZE];
    int boardSize = inputInteger(MIN_SIZE, MAX_SIZE);

    bool vsComputer = isVsComputer();

    initializeBoard(board, boardSize);

    char winner = '\0';

    MARK turn = MARK::X;
    string pos;
    memory computerMemory;
    initMemory(&computerMemory, boardSize);
    do {
        cout << endl;
        printBoard(board, boardSize);
        cout << static_cast<char>(turn) << "'s Turn: ";
        if (turn == MARK::X) {
            inputPos(&pos);
            if (mark(board, boardSize, pos, MARK::X))
                turn = MARK::O;
        } else if (turn == MARK::O) {
            if (vsComputer) {
                smartMove(board, boardSize, &computerMemory);
                turn = MARK::X;
            } else {
                inputPos(&pos);
                if (mark(board, boardSize, pos, MARK::O)) {
                    turn = MARK::X;
                }
            }
        }
    } while (!isGameFinished(board, boardSize, &winner));

    printBoard(board, boardSize);

    cout << "Winner is: " << winner << endl;
    return 0;
}


void inputPos(string *pos) {
    string s;
    cin >> s;
    if (s.length() == 1) { // whitespace check
        string y;
        cin >> y;
        *pos = s + y;
    } else {
        *pos = s;
    }
}

bool isVsComputer() {
    cout << "1) Two player\n"
         << "2) Versus computer\n"
         << "Choose game mode: ";
    int result = inputInteger(1, 2);
    return result == 2;
}

bool mark(char board[MAX_SIZE][MAX_SIZE], int boardSize, string pos, MARK markType) {
    int x = charToIndex(pos[0]);
    int y = strToInt(pos.substr(1)) - 1; // -1: board starts from 1

    if (x >= boardSize || y >= boardSize) {
        cerr << "Attempted to mark '" << pos << "', but its out of the board." << endl;
        return false;
    } else if (board[x][y] != '.') {
        cerr << "Attempted to mark '" << pos << "' but its already marked." << endl;
        return false;
    }

    board[x][y] = markType;
    return true;
}

void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int boardSize) {
    for (int i = 0; i < boardSize; ++i)
        for (int j = 0; j < boardSize; ++j)
            board[i][j] = '.';
}

void initializeVisited(bool arr[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            arr[i][j] = false;
}

void printBoard(char board[MAX_SIZE][MAX_SIZE], int boardSize) {
    // print chars
    cout << "  " << YELLOW;
    char c = 'a';
    for (int i = 0; i < boardSize; ++i, ++c)
        cout << c << ' ';
    cout << endl << RESET;
    // first blue line
    cout << "  ";
    for (int i = 0; i <= boardSize; ++i) cout << CYAN << "--";
    cout << RESET << endl;
    // print rest
    for (int i = 0; i < boardSize; ++i) {
        cout << YELLOW;
        cout << i + 1 << ' ';
        cout << RESET;
        int k = 0;
        if (i >= 9) k = 1; // to optimize spaces for 2 digit numbers
        for (; k < i; ++k) cout << ' '; // add leading spaces
        cout << RED << "\\ " << RESET;
        for (int j = 0; j < boardSize; ++j) {
            if (board[j][i] == 'x' || board[j][i] == 'X')
                cout << RED;
            else if (board[j][i] == 'o' || board[j][i] == 'O')
                cout << CYAN;
            cout << board[j][i] << ' ';
            cout << RESET;
        }
        cout << RED << "\\" << RESET;
        cout << endl;
    }
    // last blue line
    for (int i = 0; i < boardSize + 3; ++i) cout << ' ';
    for (int i = 0; i <= boardSize; ++i) cout << CYAN << "--";
    cout << RESET << endl;
}

int inputInteger(int lowerBoundary, int upperBoundary) {
    int input;
    do {
        cin >> input;
        if (input < lowerBoundary || input > upperBoundary)
            cout << "Not valid, renter: ";

    } while (input < lowerBoundary || input > upperBoundary);
    return input;
}


// check if game finished, and sets winner to x, o, or t (tie; when board full).
bool isGameFinished(char board[MAX_SIZE][MAX_SIZE], int boardSize, char *winner) {
    bool xFinished = checkFinished(board, boardSize, MARK::X);
    bool oFinished = checkFinished(board, boardSize, MARK::O);
    bool boardFull = isBoardFull(board, boardSize);

    if (xFinished) *winner = MARK::X;
    else if (oFinished) *winner = MARK::O;
    else if (boardFull) *winner = 't';

    return xFinished || oFinished || boardFull;
}

bool isBoardFull(char board[MAX_SIZE][MAX_SIZE], int boardSize) {
    for (int i = 0; i < boardSize; ++i)
        for (int j = 0; j < boardSize; ++j)
            if (board[i][j] == '.') return false;

    return true;
}

bool checkFinished(char board[MAX_SIZE][MAX_SIZE], int boardSize, MARK markType) {
    bool visited[MAX_SIZE][MAX_SIZE];
    pos start;
    start.x = start.y = 0;

    int *ptrMark = nullptr;

    // set the correct coordinate to be iterated
    switch (markType) {
        case MARK::X:
            ptrMark = &(start.y);
            break;
        case MARK::O:
            ptrMark = &(start.x);
    }

    // loop through either x or y coordinate to check starting points are filled
    for (int i = 0; i < boardSize; ++i) {
        initializeVisited(visited, boardSize);
        stack path;
        if (isConnected(start, boardSize, board, visited, markType, &path)) {
            capitalizePath(board, path, markType);
            return true;
        }
        (*ptrMark)++;
    }
    return false;
}

void capitalizePath(char board[MAX_SIZE][MAX_SIZE], stack path, MARK mark) {
    char c = 'A' - 'a' + mark;
    do {
        pos p = pop(&path);
        board[p.x][p.y] = c;
    } while (path.top > 0);
}

bool isConnected(pos p, int boardSize, char board[MAX_SIZE][MAX_SIZE], bool visited[MAX_SIZE][MAX_SIZE], MARK markType,
                 stack *path) {
    if (visited[p.x][p.y]) return false;
    visited[p.x][p.y] = true;

    if (board[p.x][p.y] != markType)
        return false;

    int border; // left side for x, bottom side for o
    switch (markType) {
        case MARK::X:
            border = p.x;
            break;
        case MARK::O:
            border = p.y;
    }
    if (border == boardSize - 1 && board[p.x][p.y] == markType) {
        push(path, p);
        return true;
    }

    pos neighbors[6];
    fillNeighbors(neighbors, p.x, p.y);
    bool res = false;
    for (int i = 0; i < 6; ++i)
        if (checkInsideOfBoard(neighbors[i], boardSize))
            res = res || isConnected(neighbors[i], boardSize, board, visited, markType, path);

    if (res) {
        push(path, p);
    }

    return res;
}


