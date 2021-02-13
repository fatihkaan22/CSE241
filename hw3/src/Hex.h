#ifndef UNTITLED1_HEX_H
#define UNTITLED1_HEX_H

#include <vector>
#include <string>
#include <fstream>
#include "ioHandler.h"
#include "Pos.h"
#include "Stack.h"

enum player {
    playerX = 'x', playerO = 'o'
};

enum cellStatus {
    empty = '.', c_x = 'x', c_o = 'o', c_X = 'X', c_O = 'O'
};

enum memoryStatus {
    defense, attack
};


class Hex {
private:

    class Cell {
    private:
        int x, y;
        cellStatus status;
    public:
        Cell(int x, int y, cellStatus status);

        Cell(int x, int y);

        int getX() const;

        int getY() const;

        cellStatus getStatus() const;

        void setX(int x);

        void setY(int y);

        void setStatus(cellStatus status);

        void set(int x, int y);

        inline friend std::ostream &operator<<(std::ostream &stream, const Cell &c) {
            return stream << static_cast<char>(c.getStatus());
        }

    };


    class FileIO {
    public:

        bool loadFile(const std::string &fileName, Hex &hex);

        // returns true if there is error on file opening
        bool saveFile(const std::string &fileName, const Hex &hex);

    private:
        std::ifstream inStream;
        std::ofstream outStream;

        static std::string getDataFromLine(const std::string &line);

        // Overloaded load functions for different data types
        void loadData(std::vector<std::vector<Cell >> &data, int size);

        void loadData(player &data);

        void loadData(bool &data);

        static std::string boardToStr(const Hex &hex);

        Stack loadStackData(int size);

        int loadIntData();

        memoryStatus loadMemStatusData();


        void saveData(const std::string &identifier, const int &data);

        void saveData(const std::string &identifier, const std::string &data);

        void saveData(const std::string &identifier, const bool &data);

        void saveData(const std::string &identifier, const char &data);
    };

    static const int MIN_SIZE = 5;
    std::vector<std::vector<Cell> > hexCells;
    bool vsComputer;
    int boardSize;
    player turn;
    char winner;
    bool gameStalled;
    int gameId;

    void initializeBoard();

    bool checkInsideOfBoard(const Pos &p) const;

    bool isAvailable(int x, int y) const;

    std::vector<Pos> getNeighbors(Pos p) const;

    bool play(std::string pos, player p);

    bool play(Pos pos, player p);

    void printBoard();

// check if game finished, and sets winner to x, o, or t (tie; when board full).
    bool isGameFinished();

    bool isBoardFull();

    bool checkFinished(player p);

    void capitalizePath(Stack &path, player playerType);

    bool isConnected(const Pos &p, std::vector<std::vector<bool>> &visited, const player &playerType, Stack &path);

    void initializeVisited(std::vector<std::vector<bool>> &visited);

    Stack opponentPos;
    memoryStatus status;
    Pos computerLastMove;
    Pos nextMove;

// push last opponent's move to stack
    void updateOpponentPos();

// every position has 6 bridges unless those are not out of the map
// however this will consider only 1 which is closer the opponents border
    Pos getLastMove() { return opponentPos.peek(); }

    bool blockPossibleBridge(const Pos &lastMove);

    bool blockOpponentsWay();

// tries to connect one side to other
    bool tryConnection();

// tries to fill neighbors of last move (not very smart but still better than random)
    bool tryNeighbors();

// finds all free positions in the board, and randomly puts the mark one of these positions
    void randomMove();

    memoryStatus getStatus() const;

    const Pos &getComputerLastMove() const;

    const Pos &getNextMove() const;

// returns an integer between start and end
    static int random(int end, int start = 0);

    Stack getOpponentPos() const { return opponentPos; }

    void smartMove();

public:

    static int nextId;
    static int activeGameId;

    Hex(bool vsComputer, int boardSize);

    Hex();

    int getBoardSize() const;

    int getBoardWidth() const { return boardSize; }

    int getBoardHeight() const { return boardSize; }

    bool isGameEnded() const { return winner != '\0'; }

    void setBoardSize(int boardSize);

    std::vector<std::vector<Cell> > getHexCells() const;

    void setHexCellStatus(int x, int y, cellStatus c);

    void setHexCellStatus(Pos p, cellStatus c);

    cellStatus getHexCellStatus(int x, int y) const;

    cellStatus getHexCellStatus(Pos p) const;

    void playGame();

    static bool isVsComputerInput();

    static int boardSizeInput();

    void handleSignal(int signal);

    int noMarked() const;

    static int noMarkedIn(const std::vector<Hex*> &games);

    bool compareGame(const Hex other) const {
        return noMarked() > other.noMarked();
    }

    static std::vector<Hex *> games;

};

#endif //UNTITLED1_HEX_H