#include "Hex.h"
#include <iostream>
#include <random>

using namespace std;

Hex::Cell::Cell(int x, int y) : x(x), y(y), status(empty) {}

Hex::Cell::Cell(int x, int y, cellStatus status) : x(x), y(y), status(status) {}

Hex::Cell::Cell() : x(0), y(0), status(empty) {}

int Hex::Cell::getX() const { return x; }

int Hex::Cell::getY() const { return y; }

void Hex::Cell::setX(int x) { Cell::x = x; }

void Hex::Cell::setY(int y) { Cell::y = y; }

void Hex::Cell::set(int x, int y) {
  Cell::x = x;
  Cell::y = y;
}

cellStatus Hex::Cell::getStatus() const { return status; }

void Hex::Cell::setStatus(cellStatus cellStatus) { status = cellStatus; }

Hex::Hex(bool vsComputer, int size)
    : vsComputer(vsComputer), boardSize(size), turn(playerX), winner('\0'),
      gameStalled(false), nextMove(size / 2, 0), gameId(Hex::nextId++),
      computerLastMove(0, 0), scoreX(0), scoreO(0) {
  initializeBoard();
}

Hex::Hex()
    : turn(playerX), winner('\0'), gameStalled(false), computerLastMove(0, 0),
      scoreX(0), scoreO(0) {
  boardSize = boardSizeInput();
  vsComputer = isVsComputerInput();
  nextMove = Pos(boardSize / 2, 0);
  gameId = Hex::nextId++;
  initializeBoard();
}

int Hex::getBoardSize() const { return boardSize; }

void Hex::setBoardSize(int size) { boardSize = size; }

void Hex::initializeBoard() {
  hexCells = new Cell *[boardSize];

  for (int i = 0; i < boardSize; i++) {
    hexCells[i] = new Cell[boardSize];
    for (int j = 0; j < boardSize; j++) {
      hexCells[i][j].setX(i);
      hexCells[i][j].setY(j);
    }
  }
}

Hex::Cell **Hex::getHexCells() const { return hexCells; }

void Hex::setHexCellStatus(int x, int y, cellStatus status) {
  hexCells[x][y].setStatus(status);
}

void Hex::setHexCellStatus(Pos p, cellStatus status) {
  setHexCellStatus(p.getX(), p.getY(), status);
}

cellStatus Hex::getHexCellStatus(int x, int y) const {
  return hexCells[x][y].getStatus();
}

cellStatus Hex::getHexCellStatus(Pos p) const {
  return getHexCellStatus(p.getX(), p.getY());
}

// checks if given position inside of board
bool Hex::checkInsideOfBoard(const Pos &p) const {
  return p.getX() >= 0 && p.getX() < boardSize && p.getY() >= 0 &&
         p.getY() < boardSize;
}

// checks if given position is haven't marked before and inside of the board
bool Hex::isAvailable(int x, int y) const {
  Pos p(x, y);
  return checkInsideOfBoard(p) &&
         getHexCellStatus(p.getX(), p.getY()) == cellStatus::empty;
}

// returns a Pos vector according to given x, y values.
// at the end arr will include all the cells that neighbours of cell of x,y
// coordinates.
vector<Pos> Hex::getNeighbors(Pos p) const {
  vector<Pos> neigbors;
  int x = p.getX();
  int y = p.getY();
  neigbors.push_back(Pos(x, y - 1));
  neigbors.push_back(Pos(x, y + 1));
  neigbors.push_back(Pos(x - 1, y));
  neigbors.push_back(Pos(x + 1, y));
  neigbors.push_back(Pos(x - 1, y + 1));
  neigbors.push_back(Pos(x + 1, y - 1));
  return neigbors;
}

bool Hex::isVsComputerInput() {
  std::cout << "1) Two player\n"
            << "2) Versus computer\n"
            << "Choose game mode: ";
  int result = ioHandler::inputInteger(1, 2);
  return result == 2;
}

bool Hex::play(Pos pos, player p) {
  if (pos.getX() >= boardSize || pos.getY() >= boardSize || pos.getX() < 0 ||
      pos.getY() < 0) {
    cerr << "Attempted to mark '[" << pos.getX() << ',' << pos.getY()
         << "]', but its out of the board." << endl;
    return false;
  } else if (hexCells[pos.getX()][pos.getY()].getStatus() !=
             cellStatus::empty) {
    cerr << "Attempted to mark '[" << pos.getX() << ',' << pos.getY()
         << "]' but its already marked." << endl;
    return false;
  }
  cellStatus c = (p == playerX) ? c = c_x : c = c_o;
  hexCells[pos.getX()][pos.getY()].setStatus(c);
  history.push(pos); // for undo
  turn = (p == playerX) ? playerO : playerX;
  return true;
}

bool Hex::play(string pos, player p) {
  int x = ioHandler::charToIndex(pos[0]);
  int y = ioHandler::strToInt(pos.substr(1)) - 1; // -1: board starts from 1
  return play(Pos(x, y), p);
}

void Hex::printBoard() {
  calculateScore();
  cout << "\nCurrent game: " << gameId << endl;
  cout << "Active games: [ ";
  for (auto i : Hex::games)
    cout << i->gameId << " | ";
  cout << "\b\b]\n";

  // print score
  cout << "scores: [x:" << scoreX << " o:" << scoreO << "]\n";

  const string RED = "\u001b[31m";
  const string CYAN = "\u001b[36m";
  const string YELLOW = "\u001b[33m";
  const string RESET = "\u001b[0m";
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
      if (hexCells[j][i].getStatus() == cellStatus::c_x)
        cout << RED;
      else if (hexCells[j][i].getStatus() == cellStatus::c_o)
        cout << CYAN;
      cout << hexCells[j][i] << ' ';
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

bool Hex::isGameFinished() {
  bool xFinished = checkFinished(player::playerX);
  bool oFinished = checkFinished(player::playerO);
  bool boardFull = isBoardFull();

  if (xFinished) {
    winner = player::playerX;
    scoreX = boardSize + 1;
  } else if (oFinished) {
    winner = player::playerO;
    scoreO = boardSize + 1;
  } else if (boardFull)
    winner = 't';

  return xFinished || oFinished || boardFull;
}

bool Hex::isBoardFull() {
  for (int i = 0; i < boardSize; ++i)
    for (int j = 0; j < boardSize; ++j)
      if (hexCells[i][j].getStatus() == cellStatus::empty)
        return false;

  return true;
}

void Hex::calculateScore() {
  scoreX = 0;
  // for x
  for (int j = 0; j < boardSize; j++) {
    for (int i = 0; i < boardSize; i++) {
      if (getHexCellStatus(Pos(j, i)) == c_x) {
        scoreX++;
        break;
      }
    }
  }

  // for o
  scoreO = 0;
  // for x
  for (int j = 0; j < boardSize; j++) {
    for (int i = 0; i < boardSize; i++) {
      if (getHexCellStatus(Pos(i, j)) == c_o) {
        scoreO++;
        break;
      }
    }
  }
}

bool Hex::checkFinished(player playerType) {
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

void Hex::initializeVisited(vector<vector<bool>> &visited) {

  // fill board
  for (int i = 0; i < boardSize * boardSize; ++i) {
    vector<bool> row;
    for (int j = 0; j < boardSize * boardSize; ++j)
      row.push_back(false);
    visited.push_back(row);
  }
}

bool Hex::isConnected(const Pos &p, std::vector<std::vector<bool>> &visited,
                      const player &playerType, Stack &path) {
  if (visited[p.getX()][p.getY()])
    return false;
  visited[p.getX()][p.getY()] = true;

  if (hexCells[p.getX()][p.getY()].getStatus() !=
      static_cast<cellStatus>(playerType))
    return false;

  int border; // left side for x, bottom side for o
  switch (playerType) {
  case player::playerX:
    border = p.getX();
    break;
  case player::playerO:
    border = p.getY();
  }
  if (border == boardSize - 1 && hexCells[p.getX()][p.getY()].getStatus() ==
                                     static_cast<cellStatus>(playerType)) {
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

void Hex::capitalizePath(Stack &path, player playerType) {
  char c = 'A' - 'a' + playerType;
  auto status = static_cast<cellStatus>(c);
  do {
    Pos p = path.pop();
    hexCells[p.getX()][p.getY()].setStatus(status);
  } while (!path.empty());
}

int Hex::random(int end, int start) {
  random_device randomDevice;
  unsigned seed = randomDevice();
  default_random_engine rd(seed);
  return start + rd() % end;
}

void Hex::smartMove() {
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

void Hex::updateOpponentPos() {
  for (int i = 0; i < boardSize; ++i) {
    for (int j = 0; j < boardSize; ++j) {
      if (hexCells[j][i].getStatus() == cellStatus::c_x) {
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
bool Hex::blockPossibleBridge(const Pos &lastMove) {
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

bool Hex::blockOpponentsWay() {
  updateOpponentPos();
  return blockPossibleBridge(getLastMove());
}

// tries to connect one side to other
bool Hex::tryConnection() {
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

bool Hex::tryNeighbors() {
  auto neighbors =
      getNeighbors(Pos(computerLastMove.getX(), computerLastMove.getY()));
  for (auto &neighbor : neighbors)
    if (isAvailable(neighbor.getX(), neighbor.getY())) {
      setHexCellStatus(neighbor, c_o);
      history.push(neighbor); // for undo
      neighbor.print();
      computerLastMove.set(neighbor);
      return true;
    }
  return false;
}

void Hex::randomMove() {
  vector<Pos> arr;
  for (int i = 0; i < boardSize; ++i)
    for (int j = 0; j < boardSize; ++j)
      if (getHexCellStatus(i, j) == empty)
        arr.push_back(Pos(i, j));

  int r = random(arr.size() - 1);
  hexCells[arr[r].getX()][arr[r].getY()].setStatus(c_o);
  history.push(Pos(arr[r].getX(), arr[r].getY())); // for undo
  arr[r].print();
}

memoryStatus Hex::getStatus() const { return status; }

const Pos &Hex::getComputerLastMove() const { return computerLastMove; }

const Pos &Hex::getNextMove() const { return nextMove; }

void Hex::playGame() {
  string pos;

  do {
    printBoard();
    if (vsComputer && turn == playerO) {
      smartMove();
    } else {
      int signal = ioHandler::inputPos(pos);
      if (signal != ioHandler::none)
        handleSignal(signal);
      else if (isGameEnded())
        cerr << "This game already is finished!\n";
      else
        play(pos, turn);
    }
  } while (!isGameFinished() && !gameStalled);

  if (!gameStalled) {
    printBoard();
    cout << "Winner is: " << winner << endl;
  }
}

int Hex::boardSizeInput() {
  cout
      << "You cannot enter commands before entering board size and game mode\n";
  cout << "Board size: ";

  return ioHandler::inputInteger(MIN_SIZE);
}

string Hex::FileIO::getDataFromLine(const string &line) {
  return line.substr(line.find(':') + 1);
}

// Overloaded load functions for different data types
int Hex::FileIO::loadIntData(istream &inStream) {
  string line;
  getline(inStream, line);
  string strData = getDataFromLine(line);
  return stoi(strData);
}

void Hex::FileIO::loadData(Cell **&data, int size, istream &inStream) {
  string line;
  getline(inStream, line);
  string strData = getDataFromLine(line);
  int strIndex = 0;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      auto c = static_cast<cellStatus>(strData[strIndex++]);
      data[j][i].setStatus(c);
    }
  }
}

void Hex::FileIO::loadData(player &data, istream &inStream) {
  string line;
  getline(inStream, line);
  char charData = getDataFromLine(line)[0];
  data = (charData == 'x') ? playerX : playerO;
}

memoryStatus Hex::FileIO::loadMemStatusData(istream &inStream) {
  string line;
  getline(inStream, line);
  char charData = getDataFromLine(line)[0];
  return (charData == '0') ? memoryStatus::defense : memoryStatus::attack;
}

void Hex::FileIO::loadData(bool &data, istream &inStream) {
  string line;
  getline(inStream, line);
  char charData = getDataFromLine(line)[0];
  data = (charData == '1') ? true : false;
}

Stack Hex::FileIO::loadStackData(int size, istream &inStream) {
  Stack data;
  string line;
  getline(inStream, line);
  string strData = getDataFromLine(line);

  for (int i = 0; i < size; ++i) {
    int strIndex1 = strData.find(',');
    int strIndex2 = strData.find(' ');
    Pos p;
    p.setX(stoi(strData.substr(0, strIndex1)));
    p.setY(stoi(strData.substr(strIndex1 + 1, strIndex2)));
    data.push(p);
    strData = strData.substr(strIndex2 + 1);
  }
  return data;
}

// returns true if there is error
bool Hex::FileIO::loadFile(istream &inStream, Hex &hex) {
  hex.boardSize = loadIntData(inStream);
  loadData(hex.hexCells, hex.boardSize, inStream);
  loadData(hex.turn, inStream);
  loadData(hex.vsComputer, inStream);
  int histSize = loadIntData(inStream);
  hex.history = loadStackData(histSize, inStream);
  if (hex.vsComputer) {
    int size = loadIntData(inStream);
    hex.opponentPos = loadStackData(size, inStream);
    int x = loadIntData(inStream);
    int y = loadIntData(inStream);
    hex.computerLastMove.setX(x);
    hex.computerLastMove.setY(y);
    x = loadIntData(inStream);
    y = loadIntData(inStream);
    hex.nextMove = Pos(x, y);
    hex.status = loadMemStatusData(inStream);
    int compLastMoveHistSize = loadIntData(inStream);
    hex.computerLastMoveHistory = loadStackData(compLastMoveHistSize, inStream);
    int nextMoveHistSize = loadIntData(inStream);
    hex.nextMoveHistory = loadStackData(nextMoveHistSize, inStream);
  }
  return false;
}

string Hex::FileIO::boardToStr(const Hex &hex) {
  string s;
  for (int i = 0; i < hex.boardSize; ++i) {
    for (int j = 0; j < hex.boardSize; ++j) {
      s += static_cast<char>(hex.hexCells[j][i].getStatus());
    }
  }
  return s;
}

string stackToStr(Stack stack) {
  string s;
  for (int i = 0; i < stack.size(); ++i) {
    s += to_string(stack.getArray()[i].getX());
    s += ',';
    s += to_string(stack.getArray()[i].getY());
    s += ' ';
  }
  return s;
}

// Overloaded saving functions for different data types
void Hex::FileIO::saveData(const string &identifier, const int &data,
                           std::ostream &outStream) {
  outStream << identifier << ':' << data << endl;
}

void Hex::FileIO::saveData(const string &identifier, const string &data,
                           std::ostream &outStream) {
  outStream << identifier << ':' << data << endl;
}

void Hex::FileIO::saveData(const string &identifier, const bool &data,
                           std::ostream &outStream) {
  outStream << identifier << ':' << data << endl;
}

void Hex::FileIO::saveData(const string &identifier, const char &data,
                           std::ostream &outStream) {
  outStream << identifier << ':' << data << endl;
}

bool Hex::FileIO::saveFile(std::ostream &out, const Hex &hex) {
  saveData("boardSize", hex.boardSize, out);
  saveData("board", boardToStr(hex), out);
  saveData("nextTurn", static_cast<char>(hex.turn), out);
  saveData("vsComputer", hex.vsComputer, out);
  saveData("history.size", hex.history.size(), out);
  saveData("history", stackToStr(hex.history), out);

  if (hex.vsComputer) {
    saveData("computerMemory.opponentPos.top", hex.getOpponentPos().size(),
             out);
    saveData("computerMemory.opponentPos.array",
             stackToStr(hex.getOpponentPos()), out);
    saveData("computerMemory.computerLastMove.x",
             hex.getComputerLastMove().getX(), out);
    saveData("computerMemory.computerLastMove.y",
             hex.getComputerLastMove().getY(), out);
    saveData("computerMemory.nextMove.x", hex.getNextMove().getX(), out);
    saveData("computerMemory.nextMove.y", hex.getNextMove().getX(), out);
    saveData("computerMemory.status", hex.getStatus(), out);
    saveData("computerLastMoveHistory.size", hex.computerLastMoveHistory.size(),
             out);
    saveData("computerLastMoveHistory", stackToStr(hex.computerLastMoveHistory),
             out);
    saveData("nextMoveHistory.size", hex.nextMoveHistory.size(), out);
    saveData("nextMoveHistory", stackToStr(hex.nextMoveHistory), out);
  }
  return false;
}

void Hex::handleSignal(int signal) {
  if (signal == ioHandler::newGame) {
    gameStalled = true;
    activeGameId = nextId;
    return;
  } else if (signal == ioHandler::go) {
    gameStalled = true;
    int input;
    cin >> input;
    if (input > nextId) {
      cerr << "Game with specified id; does not exist.\n";
    } else {
      activeGameId = input;
    }
    return;
  } else if (signal == ioHandler::marked) {
    cout << noMarkedIn(Hex::games) << endl;
    return;
  } else if (signal == ioHandler::compare) {
    int otherId;
    cin >> otherId;
    cout << (compareGame(*games[otherId]) ? "true" : "false") << endl;
    return;
  } else if (signal == ioHandler::undo) {
    (*this)--; // undo();
    if (vsComputer)
      (*this)--; // undo();
    return;
  }

  string fileName;
  cin >> fileName;
  FileIO fileIo;
  if (signal == ioHandler::load) {
    ifstream inStream;
    inStream.open(fileName);
    if (inStream.fail()) {
      cerr << "File open failed\n";
      exit(1);
    }
    // bool fail = fileIo.loadFile(inStream, *this);
    inStream >> *this;
  }
  if (signal == ioHandler::save) {
    ofstream outStream;
    outStream.open(fileName);
    if (outStream.fail()) {
      cerr << "File open failed\n";
      exit(1);
    }
    outStream << *this; // save file
    outStream.close();
  }
  cin.ignore();
}

int Hex::nextId = 0;

int Hex::activeGameId = 0;

vector<Hex *> Hex::games({});

int Hex::noMarked() const {
  int marked = 0;
  for (int i = 0; i < boardSize; i++)
    for (int j = 0; j < boardSize; j++)
      if (hexCells[i][j].getStatus() != empty)
        marked++;

  return marked;
}

int Hex::noMarkedIn(const vector<Hex *> &games) {
  int marked = 0;
  for (auto g : games)
    marked += g->noMarked();
  return marked;
}

void Hex::undo() {
  if (history.empty()) {
    cerr << "You are at the first step!";
    return;
  }
  Pos p = history.pop();
  setHexCellStatus(p, empty);
  turn = (turn == playerX) ? playerO : playerX; // change turn
  if (vsComputer) {
    nextMove = nextMoveHistory.pop();
    computerLastMove = computerLastMoveHistory.pop();
  }
  if (isGameEnded()) {
    winner = '\0';
    decapitalizePath();
  }
}

void Hex::decapitalizePath() {
  for (size_t i = 0; i < boardSize; i++)
    for (size_t j = 0; j < boardSize; j++) {
      if (hexCells[i][j].getStatus() == c_X)
        hexCells[i][j].setStatus(c_x);
      if (hexCells[i][j].getStatus() == c_O)
        hexCells[i][j].setStatus(c_o);
    }
}

Hex Hex::operator--() { // prefix
  undo();
  return *this;
}

Hex Hex::operator--(int ignore) { // postfix
  Hex temp = *this;
  undo();
  return temp;
}

// copy constructor
Hex::Hex(const Hex &hex)
    : turn(hex.turn), winner(hex.turn), gameStalled(hex.gameStalled),
      computerLastMove(hex.computerLastMove), boardSize(hex.boardSize),
      vsComputer(hex.vsComputer), nextMove(hex.nextMove), scoreX(hex.scoreX),
      scoreO(hex.scoreO) {
  // gameId intentionally left as old value
  initializeBoard();
}

Hex &Hex::operator=(const Hex &hex) {
  if (this == &hex)
    return *this;

  delete[] hexCells;
  initializeBoard();
  vsComputer = hex.vsComputer;
  boardSize = hex.boardSize;
  turn = hex.turn;
  winner = hex.winner;
  gameStalled = hex.gameStalled;
  // gameId = hex.gameId; // FIXME: possible error
  history = hex.history;

  if (vsComputer) {
    computerLastMoveHistory = hex.computerLastMoveHistory;
    nextMoveHistory = hex.nextMoveHistory;
    opponentPos = hex.opponentPos;
    status = hex.status;
    computerLastMove = hex.computerLastMove;
    nextMove = hex.nextMove;
  }
  for (int i = 0; i < boardSize; i++) {
    hexCells[i] = new Cell[boardSize];
    for (int j = 0; j < boardSize; j++) {
      hexCells[i][j].setX(i);
      hexCells[i][j].setY(j);
    }
  }
  return *this;
}

Hex::~Hex() {
  for (int i = 0; i < boardSize; i++)
    delete[] hexCells[i];

  delete[] hexCells;
}

Stack Hex::getHistory() { return history; }

Stack Hex::getComputerLastMoveHistory() { return computerLastMoveHistory; }

Stack Hex::getNextMoveHistory() { return nextMoveHistory; }

std::ostream &operator<<(std::ostream &out, Hex &h) {
  Hex::FileIO fileIo;
  fileIo.saveFile(out, h);
  return out;
}

std::istream &operator>>(std::istream &in, Hex &h) {
  Hex::FileIO fileIo;
  fileIo.loadFile(in, h);
  return in;
}
