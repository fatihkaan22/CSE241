#ifndef UNTITLED1_HEX_H
#define UNTITLED1_HEX_H

#include "Pos.h"
#include "Stack.h"
#include "ioHandler.h"
#include <fstream>
#include <string>
#include <vector>

enum player { playerX = 'x', playerO = 'o' };

enum cellStatus { empty = '.', c_x = 'x', c_o = 'o', c_X = 'X', c_O = 'O' };

enum memoryStatus { defense, attack };

class Hex {
public:
  class Cell {
  private:
    int x, y;
    cellStatus status;

  public:
    Cell(int x, int y, cellStatus status);

    Cell(int x, int y);

    Cell();

    int getX() const;

    int getY() const;

    cellStatus getStatus() const;

    void setX(int x);

    void setY(int y);

    void setStatus(cellStatus status);

    void set(int x, int y);

    inline friend std::ostream &operator<<(std::ostream &stream,
                                           const Cell &c) {
      return stream << static_cast<char>(c.getStatus());
    }
  };

  class FileIO {
  public:
    bool loadFile(std::istream &inStream, Hex &hex);

    // returns true if there is error on file opening
    bool saveFile(std::ostream &outStream, const Hex &hex);

    // FileIO(std::ostream &, std::istream &);

  private:

    static std::string getDataFromLine(const std::string &line);

    // Overloaded load functions for different data types
    void loadData(Cell **&data, int size, std::istream&);

    void loadData(player &data, std::istream&);

    void loadData(bool &data, std::istream&);

    static std::string boardToStr(const Hex &hex);

    Stack loadStackData(int size, std::istream&);

    int loadIntData( std::istream&);

    memoryStatus loadMemStatusData(std::istream&);

    void saveData(const std::string &identifier, const int &data, std::ostream&);

    void saveData(const std::string &identifier, const std::string &data, std::ostream&);

    void saveData(const std::string &identifier, const bool &data, std::ostream&);

    void saveData(const std::string &identifier, const char &data, std::ostream&);
  };

  static const int MIN_SIZE = 5;
  Cell **hexCells;
  bool vsComputer;
  int boardSize;
  player turn;
  char winner;
  bool gameStalled;
  int gameId;
  Stack history; // for undo feature
  // for undo feature: save computer memory to be able to revert it back
  Stack computerLastMoveHistory;
  Stack nextMoveHistory;
  int scoreX;
  int scoreO;

  Stack opponentPos;
  memoryStatus status;
  Pos computerLastMove;
  Pos nextMove;

  void initializeBoard();

  bool checkInsideOfBoard(const Pos &p) const;

  bool isAvailable(int x, int y) const;

  std::vector<Pos> getNeighbors(Pos p) const;

  bool play(std::string pos, player p);

  bool play(Pos pos, player p);

  void printBoard();

  // check if game finished, and sets winner to x, o, or t (tie; when board
  // full).
  bool isGameFinished();

  bool isBoardFull();

  bool checkFinished(player p);

  void capitalizePath(Stack &path, player playerType);

  bool isConnected(const Pos &p, std::vector<std::vector<bool>> &visited,
                   const player &playerType, Stack &path);

  void initializeVisited(std::vector<std::vector<bool>> &visited);

  // push last opponent's move to stack
  void updateOpponentPos();

  // every position has 6 bridges unless those are not out of the map
  // however this will consider only 1 which is closer the opponents border
  Pos getLastMove() { return opponentPos.peek(); }

  bool blockPossibleBridge(const Pos &lastMove);

  bool blockOpponentsWay();

  // tries to connect one side to other
  bool tryConnection();

  // tries to fill neighbors of last move (not very smart but still better than
  // random)
  bool tryNeighbors();

  // finds all free positions in the board, and randomly puts the mark one of
  // these positions
  void randomMove();

  memoryStatus getStatus() const;

  const Pos &getComputerLastMove() const;

  const Pos &getNextMove() const;

  // returns an integer between start and end
  static int random(int end, int start = 0);

  Stack getOpponentPos() const { return opponentPos; }

  void smartMove();

  void undo();

  void decapitalizePath();

  void calculateScore();

public:
  static int nextId;
  static int activeGameId;

  Hex(bool vsComputer, int boardSize);

  explicit Hex();

  int getBoardSize() const;

  int getBoardWidth() const { return boardSize; }

  int getBoardHeight() const { return boardSize; }

  bool isGameEnded() const { return winner != '\0'; }

  void setBoardSize(int boardSize);

  Cell **getHexCells() const;

  void setHexCellStatus(int x, int y, cellStatus c);

  void setHexCellStatus(Pos p, cellStatus c);

  cellStatus getHexCellStatus(int x, int y) const;

  cellStatus getHexCellStatus(Pos p) const;

  void playGame();

  static bool isVsComputerInput();

  static int boardSizeInput();

  void handleSignal(int signal);

  int noMarked() const;

  static int noMarkedIn(const std::vector<Hex *> &games);

  bool compareGame(const Hex other) const {
    return noMarked() > other.noMarked();
  }
  bool operator==(const Hex &other) { return compareGame(other); }

  static std::vector<Hex *> games;

  Hex operator--(); // prefix

  Hex operator--(int); // postfix

  Hex(const Hex &hex); // copy constructor

  Hex &operator=(const Hex &hex);

  ~Hex();

  Stack getHistory();

  Stack getComputerLastMoveHistory();

  Stack getNextMoveHistory();

  friend std::ostream &operator<<(std::ostream &out, Hex &h);

  friend std::istream &operator>>(std::istream &in, Hex &h);
};

#endif // UNTITLED1_HEX_H