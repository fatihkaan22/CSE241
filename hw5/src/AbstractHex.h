#ifndef HW5_ABSTRACTHEX_H
#define HW5_ABSTRACTHEX_H

#include <string>
#include "Pos.h"
#include "Stack.h"
#include "Cell.h"

namespace FatihKaanHex {

    enum player {
        playerX = 'x', playerO = 'o'
    };

    enum memoryStatus {
        defense, attack
    };

    class AbstractHex {
    protected:
        AbstractHex();

        static const int MIN_SIZE = 5;
        char winner;
        Stack computerLastMoveHistory;
        Stack nextMoveHistory;
        Stack opponentPos;
        memoryStatus status;
        Pos computerLastMove;
        Pos nextMove;
        Pos lastMovePos;
        int boardSize;
        player turn;
        bool vsComputer;
        // for multiple games
        int gameId;
        bool gameStalled;
        static int nextId;

        void stringToHexCells(std::string board);

        std::string hexCellsToString() const;

        void setHexCellStatus(int x, int y, cellStatus c);

        cellStatus getHexCellStatus(int x, int y) const;

        void setBoardSize(int size);

        bool checkInsideOfBoard(const Pos &p) const;

        bool isAvailable(int x, int y) const;

        std::vector<Pos> getNeighbors(Pos p) const;

        bool isVsComputerInput();

        bool checkFinished(player playerType);

        void initializeVisited(std::vector<std::vector<bool>> &visited) const;

        bool isConnected(const Pos &p, std::vector<std::vector<bool>> &visited, const player &playerType, Stack &path);

        void capitalizePath(Stack &path, player playerType);

        void smartMove();

        int random(int end, int start = 0);

        void updateOpponentPos();

        bool blockPossibleBridge(const Pos &lastMove);

        void randomMove();

        bool tryNeighbors();

        bool tryConnection();

        static int boardSizeInput();

        bool blockOpponentsWay();

        Pos getLastMove();

        bool isGameFinished();

        Stack getOpponentPos() const { return opponentPos; }

        const Pos &getComputerLastMove() const { return computerLastMove; }

        const Pos &getNextMove() const { return nextMove; }

        memoryStatus getStatus() const { return status; }


    public:

        AbstractHex(bool vsComputer, int size);

        void print() const;

        void readFromFile(std::istream &inStream);

        void writeToFile(std::ostream &outStream) const;

        void reset();

        virtual void setSize(int size);

        bool play(); // smart move

        bool play(std::string pos, player p);

        bool play(Pos pos, player p);

        bool isEnd() const;

        virtual Cell *operator()(int x, int y) const = 0;

        bool operator==(const AbstractHex &other) const;

        Pos lastMove() const;

        int numberOfMoves() const;

        bool isBoardFull() const;

        void playGame();

        void checkPos(int x, int y) const;

        int getBoardSize() const;

        player getTurn() { return turn; };

        int getGameId() { return gameId; };

        int getGameStalled() { return gameStalled; };

        int getNextId() { return nextId; };

        void setGameStalled(bool b) { gameStalled = b; };

        static int activeGameId;

        ~AbstractHex();
    };
}
#endif //HW5_ABSTRACTHEX_H
