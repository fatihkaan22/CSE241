#include <iostream>
#include <string>

#include "convert.h"
#include "computerMoves.h"
#include "gameStructures.h"
#include "ioHandler.h"

#define RED "\u001b[31m"
#define CYAN "\u001b[36m"
#define YELLOW "\u001b[33m"
#define RESET "\u001b[0m"

using namespace std;

// simple function that reads integer from cin between given boundaries
int inputInteger(const int &lowerBoundary, const int &upperBoundary);

bool isVsComputer();

void printBoard(const CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize);

// creates a board with given size, and fills with . character
void initializeBoard(CELL (&board)[MAX_SIZE][MAX_SIZE], const int &boardSize);

// marks either x or o according to markType, if given position is successfully marked returns true, false otherwise
bool mark(CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize, const string &pos, const MARK &markType);

// checks if given position connected to the other side of the board
bool isConnected(const pos &p, const int &boardSize, const CELL board[MAX_SIZE][MAX_SIZE],
                 bool visited[MAX_SIZE][MAX_SIZE], const MARK &markType, stack &path);

// sets false all the elements in arr
void initializeVisited(bool (&arr)[MAX_SIZE][MAX_SIZE], const int &size);

// checks if one of the users finished the game, returns true if the case
bool checkFinished(CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize, const MARK &markType);

// checks either one of the users is finished the game or the board is full
bool isBoardFull(const CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize);

bool isGameFinished(CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize, char &winner);

void capitalizePath(CELL board[MAX_SIZE][MAX_SIZE], stack &path, const MARK &mark);

void handleSignal(const int &signal, CELL board[MAX_SIZE][MAX_SIZE], int &boardSize, bool &vsComputer, MARK &nextTurn,
                  memory &computerMemory);

// gets position from cin with some validations
int inputPos(string &pos);

int main() {
    cout << "You cannot enter load and save commands before entering board size and game mode\n";
    cout << "Board size: ";
    CELL board[MAX_SIZE][MAX_SIZE];

    int boardSize = inputInteger(MIN_SIZE, MAX_SIZE);

    bool vsComputer = isVsComputer();

    initializeBoard(board, boardSize);

    char winner = '\0';

    MARK turn = MARK::X;
    string pos;
    memory computerMemory;
    initMemory(computerMemory, boardSize);

//    cout << boardSize << endl;
//
//    for (int i = 0; i < boardSize; ++i) {
//        for (int j = 0; j < boardSize; ++j) {
//            cout << static_cast<char>(board[j][i]);
//        }
//    }
//    cout << endl;
//    cout << turn << endl;
//    cout << vsComputer << endl;
//    cout << computerMemory.opponentPos.top << endl;
//
//    for (int i = 0; i < computerMemory.opponentPos.top; ++i) {
//        cout << computerMemory.opponentPos.array[i].x;
//        cout << ',';
//        cout << computerMemory.opponentPos.array[i].y;
//        cout << ' ';
//    }
//    cout << endl;

    do {
        cout << endl;
        printBoard(board, boardSize);
        cout << static_cast<char>(turn) << "'s Turn: ";
        if (turn == MARK::X) {
            int signal = inputPos(pos);
            if (signal != 0) // either load or save
                handleSignal(signal, board, boardSize, vsComputer, turn, computerMemory);
            else if (mark(board, boardSize, pos, MARK::X))
                turn = MARK::O;
        } else if (turn == MARK::O) {
            if (vsComputer) {
                smartMove(board, boardSize, computerMemory);
                turn = MARK::X;
            } else {
                int signal = inputPos(pos);
                if (signal != 0) // either load or save
                    handleSignal(signal, board, boardSize, vsComputer, turn, computerMemory);
                else if (mark(board, boardSize, pos, MARK::O))
                    turn = MARK::X;
            }
        }
//        saveFile(board, boardSize, vsComputer, turn, computerMemory);
    } while (!isGameFinished(board, boardSize, winner));

    printBoard(board, boardSize);

    cout << "Winner is: " << winner << endl;
    return 0;
}

void handleSignal(const int &signal, CELL board[MAX_SIZE][MAX_SIZE], int &boardSize, bool &vsComputer, MARK &turn,
                  memory &computerMemory) {
    string fileName;
    cin >> fileName;
    if (signal == 1) {
        bool fail = loadFile(fileName, board, boardSize, vsComputer, turn, computerMemory);
        if (!fail) cout << "Loaded!\n";
        else exit(1);
    }
    if (signal == 2) {
        bool fail = saveFile(fileName, board, boardSize, vsComputer, turn, computerMemory);
        if (!fail) cout << "Saved!\n";
        else exit(1);
    }
    cin.ignore(); //FIXME
}

// return 0: if position, 1: if load, 2 if save
int inputPos(string &pos) {
    string s;
    cin >> s;

    if (s == "load" || s == "LOAD") return 1;
    else if (s == "save" || s == "SAVE") return 2;
    else if (s.length() == 1) { // whitespace check
        string y;
        cin >> y;
        pos = s + y;
    } else {
        pos = s;
    }
    return 0;
}

bool isVsComputer() {
    cout << "1) Two player\n"
         << "2) Versus computer\n"
         << "Choose game mode: ";
    int result = inputInteger(1, 2);
    return result == 2;
}

bool mark(CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize, const string &pos, const MARK &markType) {
    int x = charToIndex(pos[0]);
    int y = strToInt(pos.substr(1)) - 1; // -1: board starts from 1

    if (x >= boardSize || y >= boardSize || x < 0 || y < 0) {
        cerr << "Attempted to mark '" << pos << "', but its out of the board." << endl;
        return false;
    } else if (board[x][y] != EMPTY_CELL) {
        cerr << "Attempted to mark '" << pos << "' but its already marked." << endl;
        return false;
    }

    board[x][y] = static_cast<CELL>(markType);
    return true;
}

void initializeBoard(CELL (&board)[MAX_SIZE][MAX_SIZE], const int &boardSize) {
    for (auto &i : board)
        for (auto &j : i)
            j = CELL::EMPTY_CELL;
}

void initializeVisited(bool (&arr)[MAX_SIZE][MAX_SIZE], const int &size) {
    for (decltype(arr[0]) i : arr)
        for (decltype(i[0]) j : i)
            j = false;
}

void printBoard(const CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize) {
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
            if (board[j][i] == X_CELL || board[j][i] == X_CELL_CAPITAL)
                cout << RED;
            else if (board[j][i] == O_CELL || board[j][i] == O_CELL_CAPITAL)
                cout << CYAN;
            cout << static_cast<char>(board[j][i]) << ' ';
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

int inputInteger(const int &lowerBoundary, const int &upperBoundary) {
    int input;
    do {
        cin >> input;
        if (input < lowerBoundary || input > upperBoundary)
            cout << "Not valid, renter: ";

    } while (input < lowerBoundary || input > upperBoundary);
    return input;
}


// check if game finished, and sets winner to x, o, or t (tie; when board full).
// board shouldn't be const here, since we need to capitalize path in checkFinished(..)
bool isGameFinished(CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize, char &winner) {
    bool xFinished = checkFinished(board, boardSize, MARK::X);
    bool oFinished = checkFinished(board, boardSize, MARK::O);
    bool boardFull = isBoardFull(board, boardSize);

    if (xFinished) winner = MARK::X;
    else if (oFinished) winner = MARK::O;
    else if (boardFull) winner = 't';

    return xFinished || oFinished || boardFull;
}

bool isBoardFull(const CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize) {
    for (int i = 0; i < boardSize; ++i)
        for (int j = 0; j < boardSize; ++j)
            if (board[i][j] == EMPTY_CELL) return false;

    return true;
}

bool checkFinished(CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize, const MARK &markType) {
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
        if (isConnected(start, boardSize, board, visited, markType, path)) {
            capitalizePath(board, path, markType);
            return true;
        }
        (*ptrMark)++;
    }
    return false;
}

void capitalizePath(CELL board[MAX_SIZE][MAX_SIZE], stack &path, const MARK &mark) {
    char c = 'A' - 'a' + mark;
    CELL cell = static_cast<CELL>(c);
    do {
        pos p = pop(path);
        board[p.x][p.y] = cell;
    } while (path.top > 0);
}

bool isConnected(const pos &p, const int &boardSize, const CELL board[MAX_SIZE][MAX_SIZE],
                 bool visited[MAX_SIZE][MAX_SIZE], const MARK &markType, stack &path) {
    if (visited[p.x][p.y]) return false;
    visited[p.x][p.y] = true;

    if (board[p.x][p.y] != static_cast<CELL>(markType))
        return false;

    int border; // left side for x, bottom side for o
    switch (markType) {
        case MARK::X:
            border = p.x;
            break;
        case MARK::O:
            border = p.y;
    }
    if (border == boardSize - 1 && board[p.x][p.y] == static_cast<CELL>(markType)) {
        push(path, p);
        return true;
    }

    pos neighbors[6];
    fillNeighbors(neighbors, p.x, p.y);
    bool res = false;
    for (auto neighbor : neighbors)
        if (checkInsideOfBoard(neighbor, boardSize))
            res = res || isConnected(neighbor, boardSize, board, visited, markType, path);

    if (res) {
        push(path, p);
    }

    return res;
}