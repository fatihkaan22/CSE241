#include "AbstractHex.h"
#include "FileIO.h"
#include <iostream>
#include <random>
#include <sstream>
#include "ioHandler.h"

using namespace std;

namespace FatihKaanHex {

    AbstractHex::AbstractHex(bool vsComputer, int size) : vsComputer(vsComputer), boardSize(size), turn(playerX),
                                                          winner('\0'),
                                                          gameStalled(false), nextMove(size / 2, 0),
                                                          gameId(AbstractHex::nextId++),
                                                          computerLastMove(0, 0),
                                                          lastMovePos(Pos(-1, -1)) {/* Intentionally empty */}

    AbstractHex::AbstractHex() : AbstractHex(isVsComputerInput(), boardSizeInput()) {
        /* Intentionally empty */
    }


    int AbstractHex::getBoardSize() const { return boardSize; }

    void AbstractHex::setBoardSize(int size) { boardSize = size; }

// checks if given position inside of board
    bool AbstractHex::checkInsideOfBoard(const Pos &p) const {
        return p.getX() >= 0 && p.getX() < boardSize && p.getY() >= 0 &&
               p.getY() < boardSize;
    }

// checks if given position inside of board & throws exception if illegal
    void AbstractHex::checkPos(int x, int y) const {
        if (!checkInsideOfBoard(Pos(x, y))) {
            throw range_error("Invalid x,y");
        }
    }


// checks if given position is haven't marked before and inside of the board
    bool AbstractHex::isAvailable(int x, int y) const {
        Pos p(x, y);
        return checkInsideOfBoard(p) &&
               getHexCellStatus(p.getX(), p.getY()) == cellStatus::empty;
    }

// returns a Pos vector according to given x, y values.
// at the end arr will include all the cells that neighbours of cell of x,y
// coordinates.
    vector<Pos> AbstractHex::getNeighbors(Pos p) const {
        vector<Pos> neighbors;
        int x = p.getX();
        int y = p.getY();
        neighbors.push_back(Pos(x, y - 1));
        neighbors.push_back(Pos(x, y + 1));
        neighbors.push_back(Pos(x - 1, y));
        neighbors.push_back(Pos(x + 1, y));
        neighbors.push_back(Pos(x - 1, y + 1));
        neighbors.push_back(Pos(x + 1, y - 1));
        return neighbors;
    }

    bool AbstractHex::isVsComputerInput() {
        std::cout << "1) Two player\n"
                  << "2) Versus computer\n"
                  << "Choose game mode: ";
        int result = ioHandler::inputInteger(1, 2);
        return result == 2;
    }

    void AbstractHex::print() const {
#ifdef no_color_hex
        const string RED = "";
        const string CYAN = "";
        const string YELLOW = "";
        const string RESET = "";
#else
        const string RED = "\u001b[31m";
        const string CYAN = "\u001b[36m";
        const string YELLOW = "\u001b[33m";
        const string RESET = "\u001b[0m";
#endif
        // print chars
        cout << "  " << YELLOW;
        char c = 'a';
        for (int i = 0; i < boardSize; ++i, ++c)
            cout << c << ' ';
        cout << endl << RESET;
        // first blue line
        cout << "  ";
        for (int i = 0; i <= boardSize; ++i)
            cout << CYAN << "--";
        cout << RESET << endl;
        // print rest
        for (int i = 0; i < boardSize; ++i) {
            cout << YELLOW;
            cout << i + 1 << ' ';
            cout << RESET;
            int k = 0;
            if (i >= 9)
                k = 1; // to optimize spaces for 2 digit numbers
            for (; k < i; ++k)
                cout << ' '; // add leading spaces
            cout << RED << "\\ " << RESET;
            for (int j = 0; j < boardSize; ++j) {
                if (getHexCellStatus(j, i) == cellStatus::c_x)
                    cout << RED;
                else if (getHexCellStatus(j, i) == cellStatus::c_o)
                    cout << CYAN;
                cout << static_cast<char>(getHexCellStatus(j, i)) << ' ';
                cout << RESET;
            }
            cout << RED << "\\" << RESET;
            cout << endl;
        }
        // last blue line
        for (int i = 0; i < boardSize + 3; ++i)
            cout << ' ';
        for (int i = 0; i <= boardSize; ++i)
            cout << CYAN << "--";
        cout << RESET << endl;
        cout << static_cast<char>(turn) << "'s Turn: ";
    }


    bool AbstractHex::isBoardFull() const {
        for (int i = 0; i < boardSize; ++i)
            for (int j = 0; j < boardSize; ++j)
                if (getHexCellStatus(i, j) == cellStatus::empty)
                    return false;

        return true;
    }

    bool AbstractHex::isGameFinished() {
        bool xFinished = checkFinished(player::playerX);
        bool oFinished = checkFinished(player::playerO);
        bool boardFull = isBoardFull();


        if (xFinished) {
            winner = player::playerX;
        } else if (oFinished) {
            winner = player::playerO;
        } else if (boardFull)
            winner = 't';

        return xFinished || oFinished || boardFull;
    }

    bool AbstractHex::checkFinished(player playerType) {
        vector<vector<bool>> visited;
        Pos start(0, 0);

        // loop through either x or y coordinate to check starting points are filled
        for (int i = 0; i < boardSize; ++i) {
            initializeVisited(visited);
            Stack path;
            if (isConnected(start, visited, playerType, path)) {
                capitalizePath(path, playerType);
                return true;
            }

            // iterate correct coordinate
            switch (playerType) {
                case player::playerX:
                    start.setY(start.getY() + 1);
                    break;
                case player::playerO:
                    start.setX(start.getX() + 1);
            }
        }
        return false;
    }

    void AbstractHex::initializeVisited(vector<vector<bool>> &visited) const {
        // fill board
        for (int i = 0; i < boardSize * boardSize; ++i) {
            vector<bool> row;
            for (int j = 0; j < boardSize * boardSize; ++j)
                row.push_back(false);
            visited.push_back(row);
        }
    }

    bool AbstractHex::isConnected(const Pos &p, std::vector<std::vector<bool>> &visited,
                                  const player &playerType, Stack &path) {
        if (visited[p.getX()][p.getY()])
            return false;
        visited[p.getX()][p.getY()] = true;

        if (getHexCellStatus(p.getX(), p.getY()) != static_cast<cellStatus>(playerType)) {
            return false;
        }


        int border; // left side for x, bottom side for o
        cellStatus c;
        switch (playerType) {
            case player::playerX:
                border = p.getX();
                break;
            case player::playerO:
                border = p.getY();
        }

        if (border == boardSize - 1 && getHexCellStatus(p.getX(), p.getY()) == static_cast<cellStatus>(playerType)) {
            path.push(p);
            return true;
        }

        vector<Pos> neighbors = getNeighbors(p);
        bool res = false;
        for (auto neighbor : neighbors)
            if (checkInsideOfBoard(neighbor)) {
                res = res || isConnected(neighbor, visited, playerType, path);
            }

        if (res) {
            path.push(p);
        }

        return res;
    }

    void AbstractHex::capitalizePath(Stack &path, player playerType) {
        char c = 'A' - 'a' + playerType;
        auto status = static_cast<cellStatus>(c);
        do {
            Pos p = path.pop();
            setHexCellStatus(p.getX(), p.getY(), status);
        } while (!path.empty());
    }

    int AbstractHex::random(int end, int start) {
        random_device randomDevice;
        unsigned seed = randomDevice();
        default_random_engine rd(seed);
        return start + rd() % end;
    }

    void AbstractHex::smartMove() {
        // save state for undo:
        nextMoveHistory.push(nextMove);
        computerLastMoveHistory.push(computerLastMove);
        bool played = false;


        switch (random(2)) {
            case 0:
                status = memoryStatus::defense;
                break;
            case 1:
                status = memoryStatus::attack;
        }

        if (status == memoryStatus::defense)
            played = blockOpponentsWay();
        if (!played && status == memoryStatus::attack)
            played = tryConnection();
        if (!played)
            played = tryNeighbors();
        if (!played)
            randomMove();
        turn = playerX;
    }

    void AbstractHex::updateOpponentPos() {
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (getHexCellStatus(j, i) == cellStatus::c_x) {
                    Pos p(j, i);
                    if (!opponentPos.exist(p)) {
                        opponentPos.push(p);
                        break;
                    }
                }
            }
        }
    }

// x, (y+2): same
// x, (y-2): same
// (x+2), (y-1): right
// (x+1), (y+1): right
// (x-2), (y+1): left
// (x-1), (y-1): left
    bool AbstractHex::blockPossibleBridge(const Pos &lastMove) {
        char border = (lastMove.getX() < getBoardSize() / 2)
                      ? 'l'
                      : 'r'; // left or right (closest)
        bool decided = false;
        Pos decision;
        if (border == 'l') {
            if (isAvailable(lastMove.getX() - 2, lastMove.getY() + 1)) {
                decision.setX(lastMove.getX() - 2);
                decision.setY(lastMove.getY() + 1);
                decided = true;
            } else if (isAvailable(lastMove.getX() - 1, lastMove.getY() - 1)) {
                decision.setX(lastMove.getX() - 1);
                decision.setY(lastMove.getY() - 1);
                decided = true;
            }
        } else if (border == 'r') {
            if (isAvailable(lastMove.getX() + 2, lastMove.getY() - 1)) {
                decision.setX(lastMove.getX() + 2);
                decision.setY(lastMove.getY() - 1);
                decided = true;
            } else if (isAvailable(lastMove.getX() + 1, lastMove.getY() + 1)) {
                decision.setX(lastMove.getX() + 1);
                decision.setY(lastMove.getY() + 1);
                decided = true;
            }
        }
        if (decided) {
            play(decision, playerO);
            decision.print();
            computerLastMove.setX(decision.getX());
            computerLastMove.setY(decision.getY());
        }
        return decided;
    }

    bool AbstractHex::blockOpponentsWay() {
        updateOpponentPos();
        return blockPossibleBridge(getLastMove());
    }

// tries to connect one side to other
    bool AbstractHex::tryConnection() {
        bool played = false;
        int x = -1, y;

        if (!isAvailable(nextMove.getX(), nextMove.getY()))
            return false;

        if (getHexCellStatus(nextMove.getX(), nextMove.getY()) == c_o ||
            getHexCellStatus(nextMove.getX(), nextMove.getY()) == empty) {
            if (isAvailable(nextMove.getX(), nextMove.getY())) {
                x = nextMove.getX();
                y = nextMove.getY();
            } else if (isAvailable(nextMove.getX(), nextMove.getY() + 1)) {
                x = nextMove.getX();
                y = nextMove.getY() + 1;
            } else if (isAvailable(nextMove.getX() - 1, nextMove.getY() + 1)) {
                x = nextMove.getX() - 1;
                y = nextMove.getY() + 1;
            }
        } else {
            if (isAvailable(nextMove.getX() + 1, nextMove.getY())) {
                x = nextMove.getX() + 1;
                y = nextMove.getY();
            } else if (isAvailable(nextMove.getX() - 1, nextMove.getY())) {
                x = nextMove.getX() - 1;
                y = nextMove.getY();
            }
        }
        if (x != -1) {
            play(Pos(x, y), playerO);
            Pos(x, y).print();
            played = true;
            nextMove.setX(x);
            nextMove.setY(y);
        }
        return played;
    }

    bool AbstractHex::tryNeighbors() {
        auto neighbors =
                getNeighbors(Pos(computerLastMove.getX(), computerLastMove.getY()));
        for (auto &neighbor : neighbors)
            if (isAvailable(neighbor.getX(), neighbor.getY())) {
                setHexCellStatus(neighbor.getX(), neighbor.getY(), c_o);
                neighbor.print();
                lastMovePos = neighbor;
                computerLastMove.set(neighbor);
                return true;
            }
        return false;
    }

    void AbstractHex::randomMove() {
        vector<Pos> arr;
        for (int i = 0; i < boardSize; ++i)
            for (int j = 0; j < boardSize; ++j)
                if (getHexCellStatus(i, j) == empty)
                    arr.push_back(Pos(i, j));

        int r = random(arr.size() - 1);
        lastMovePos = Pos(arr[r].getX(), arr[r].getY());
        setHexCellStatus(arr[r].getX(), arr[r].getY(), c_o);
        arr[r].print();
    }

    bool AbstractHex::play(Pos pos, player p) {
        if (pos.getX() >= boardSize || pos.getY() >= boardSize || pos.getX() < 0 ||
            pos.getY() < 0) {
            cerr << "Attempted to mark '[" << pos.getX() << ',' << pos.getY()
                 << "]', but its out of the board." << endl;
            return false;
        } else if (getHexCellStatus(pos.getX(), pos.getY()) != cellStatus::empty) {
            cerr << "Attempted to mark '[" << pos.getX() << ',' << pos.getY()
                 << "]' but its already marked." << endl;
            return false;
        }
        cellStatus c = (p == playerX) ? c = c_x : c = c_o;
        lastMovePos = pos;
        setHexCellStatus(pos.getX(), pos.getY(), c);
        turn = (p == playerX) ? playerO : playerX;
        return true;
    }

    bool AbstractHex::play(string pos, player p) {
        int x = ioHandler::charToIndex(pos[0]);
        int y = ioHandler::strToInt(pos.substr(1)) - 1; // -1: board starts from 1
        return play(Pos(x, y), p);
    }

    bool AbstractHex::play() {
        smartMove();
        return true;
    }

    void AbstractHex::playGame() {
        string pos;

        do {
            print();
            if (vsComputer && turn == playerO) {
                play();
            } else {
                ioHandler::signal signal = ioHandler::inputPos(pos);
                if (signal != ioHandler::signal::none)
                    ioHandler::handleSignal(signal, this);
                else if (isEnd())
                    cerr << "This game already is finished!\n";
                else
                    play(pos, turn);
            }
        } while (!isGameFinished() && !gameStalled);

        if (!gameStalled) {
            print();
            cout << "Winner is: " << winner << endl;
        }
    }

    int AbstractHex::boardSizeInput() {
        cout << "You cannot enter commands before entering board size and game mode\n";
        cout << "Board size: ";

        return ioHandler::inputInteger(MIN_SIZE);
    }

    bool AbstractHex::isEnd() const {
        return winner != '\0';
    }

    Pos AbstractHex::getLastMove() {
        return opponentPos.peek();
    }

    void AbstractHex::readFromFile(istream &inStream) {
        FileIO fileIo;
        string board;
        setBoardSize(fileIo.loadIntData(inStream));
        fileIo.loadData(board, inStream);
        stringToHexCells(board);
        fileIo.loadData(turn, inStream);
        fileIo.loadData(vsComputer, inStream);
        if (vsComputer) {
            int size = fileIo.loadIntData(inStream);
            opponentPos = fileIo.loadStackData(size, inStream);
            int x = fileIo.loadIntData(inStream);
            int y = fileIo.loadIntData(inStream);
            computerLastMove.setX(x);
            computerLastMove.setY(y);
            x = fileIo.loadIntData(inStream);
            y = fileIo.loadIntData(inStream);
            nextMove = Pos(x, y);
            status = fileIo.loadMemStatusData(inStream);
            int nextMoveHistSize = fileIo.loadIntData(inStream);
            nextMoveHistory = fileIo.loadStackData(nextMoveHistSize, inStream);
        }
    }

    void AbstractHex::stringToHexCells(std::string strData) {
        int strIndex = 0;
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                auto c = static_cast<cellStatus>(strData[strIndex++]);
                setHexCellStatus(j, i, c);
            }
        }
    }

    string AbstractHex::hexCellsToString() const {
        stringstream ss;
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                ss << static_cast<char>(getHexCellStatus(j, i));
            }
        }
        return ss.str();
    }

    void AbstractHex::writeToFile(std::ostream &out) const {

        FileIO fileIo;
        fileIo.saveData("boardSize", getBoardSize(), out);
        fileIo.saveData("board", hexCellsToString(), out);
        fileIo.saveData("nextTurn", static_cast<char>(turn), out);
        fileIo.saveData("vsComputer", vsComputer, out);

        if (vsComputer) {
            fileIo.saveData("computerMemory.opponentPos.top", getOpponentPos().size(),
                            out);
            fileIo.saveData("computerMemory.opponentPos.array",
                            FileIO::stackToStr(getOpponentPos()), out);
            fileIo.saveData("computerMemory.computerLastMove.x",
                            getComputerLastMove().getX(), out);
            fileIo.saveData("computerMemory.computerLastMove.y",
                            getComputerLastMove().getY(), out);
            fileIo.saveData("computerMemory.nextMove.x", getNextMove().getX(), out);
            fileIo.saveData("computerMemory.nextMove.y", getNextMove().getX(), out);
            fileIo.saveData("computerMemory.status", getStatus(), out);
            fileIo.saveData("nextMoveHistory.size", nextMoveHistory.size(), out);
            fileIo.saveData("nextMoveHistory", FileIO::stackToStr(nextMoveHistory), out);
        }
    }

    int AbstractHex::nextId = 0;

    void AbstractHex::reset() {
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                setHexCellStatus(j, i, cellStatus::empty);
            }
        }
    }

    int AbstractHex::numberOfMoves() const {
        int marked = 0;
        for (int i = 0; i < boardSize; i++)
            for (int j = 0; j < boardSize; j++)
                if (getHexCellStatus(j, i) != empty)
                    marked++;
        return marked;
    }


    cellStatus AbstractHex::getHexCellStatus(int x, int y) const {
        return operator()(x, y)->getStatus();
    }


    void AbstractHex::setHexCellStatus(int x, int y, cellStatus c) {
        operator()(x, y)->setStatus(c);
    }

    bool AbstractHex::operator==(const AbstractHex &other) const {
        if (boardSize != other.getBoardSize()) return false;
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (getHexCellStatus(j, i) != other.getHexCellStatus(j, i))
                    return false;
            }
        }
        return true;
    }

    void AbstractHex::setSize(int size) {
        if (size < MIN_SIZE) {
            throw invalid_argument("game size is less than MIN_SIZE (5)");
        }
        boardSize = size;
        reset();
    }


    Pos AbstractHex::lastMove() const {
        if (lastMovePos.getX() == -1)
            throw runtime_error("There is no last move.");
        return lastMovePos;
    }

    int AbstractHex::activeGameId = 0;

    AbstractHex::~AbstractHex() {
        nextId--;
    }
}
