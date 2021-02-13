import java.io.*;
import java.util.ArrayList;
import java.util.Random;

public class Hex implements HexInterface{
  Cell[][] hexCells;
  boolean vsComputer;
  int boardSize;
  Player turn;
  String winner;
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

  public Hex(boolean vsComputer, int size) {
    this.vsComputer = vsComputer;
    this.boardSize = size;
    initializeBoard();
  }

  public int getBoardSize() {
    return boardSize;
  }

  private void setBoardSize(int size) {
    boardSize = size;
  }

  void initializeBoard() {
    history = new Stack();
    nextMoveHistory = new Stack();
    computerLastMoveHistory = new Stack();
    opponentPos = new Stack();
    turn = Player.playerX;
    winner = "\0";
    nextMove = new Pos(boardSize / 2, 0);
    computerLastMove = new Pos(0, 0);
    scoreX = 0;
    scoreO = 0;

    hexCells = new Cell[boardSize][boardSize];

    for (int i = 0; i < boardSize; i++) {
      for (int j = 0; j < boardSize; j++) {
        hexCells[i][j] = new Cell(i, j);
      }
    }
  }

  public Player getTurn() {
    return turn;
  }

  private Cell[][] getHexCells() {
    return hexCells;
  }

  void setHexCellStatus(int x, int y, cellStatus status) {
    hexCells[x][y].setStatus(status);
  }

  void setHexCellStatus(Pos p, cellStatus status) {
    setHexCellStatus(p.getX(), p.getY(), status);
  }

  cellStatus getHexCellStatus(int x, int y) {
    return hexCells[x][y].getStatus();
  }

  cellStatus getHexCellStatus(Pos p) {
    return getHexCellStatus(p.getX(), p.getY());
  }

  // checks if given position inside of board
  public boolean checkInsideOfBoard(Pos p) {
    return p.getX() >= 0 && p.getX() < boardSize && p.getY() >= 0 && p.getY() < boardSize;
  }

  // checks if given position is haven't marked before and inside of the board
  public boolean isAvailable(int x, int y) {
    Pos p = new Pos(x, y);
    return checkInsideOfBoard(p) && getHexCellStatus(p.getX(), p.getY()) == cellStatus.empty;
  }

  // returns a Pos vector according to given x, y values.
  // at the end arr will include all the cells that neighbours of cell of x,y
  // coordinates.
  private ArrayList<Pos> getNeighbors(Pos p) {
    ArrayList<Pos> neighbours = new ArrayList<>();
    int x = p.getX();
    int y = p.getY();
    neighbours.add(new Pos(x, y - 1));
    neighbours.add(new Pos(x, y + 1));
    neighbours.add(new Pos(x - 1, y));
    neighbours.add(new Pos(x + 1, y));
    neighbours.add(new Pos(x - 1, y + 1));
    neighbours.add(new Pos(x + 1, y - 1));
    return neighbours;
  }

  public boolean play(Pos pos, Player p) {
    if (!checkInsideOfBoard(pos)) {
      System.err.println("Attempted to mark '[" + pos.getX() + ',' + pos.getY() + "]', but its out of the board.\n");
      return false;
    } else if (hexCells[pos.getX()][pos.getY()].getStatus() != cellStatus.empty) {
      System.err.println("Attempted to mark '[" + pos.getX() + ',' + pos.getY() + "]' but its already marked.\n");
      return false;
    }
    cellStatus c = (p == Player.playerX) ? cellStatus.c_x : cellStatus.c_o;
    hexCells[pos.getX()][pos.getY()].setStatus(c);
    history.push(new Pos(pos.getX(), pos.getY())); // for undo
    turn = (p == Player.playerX) ? Player.playerO : Player.playerX;
    return true;
  }

  public boolean play(String pos, Player p) {
    int x = Integer.parseInt(String.valueOf(pos.charAt(0)));
    int y = (Integer.parseInt(pos.substring(1))) - 1; // -1: board starts from 1
    return play(new Pos(x, y), p);
  }

  public boolean isGameFinished() {
    boolean xFinished = checkFinished(Player.playerX);
    boolean oFinished = checkFinished(Player.playerO);
    boolean boardFull = isBoardFull();

    if (xFinished) {
      winner = Player.playerX.toString();
      scoreX = boardSize + 1;
    } else if (oFinished) {
      winner = Player.playerO.toString();
      scoreO = boardSize + 1;
    } else if (boardFull)
      winner = "t";

    return xFinished || oFinished || boardFull;
  }

  public boolean isBoardFull() {
    for (int i = 0; i < boardSize; ++i)
      for (int j = 0; j < boardSize; ++j)
        if (hexCells[i][j].getStatus() == cellStatus.empty)
          return false;

    return true;
  }

  private void calculateScore() {
    scoreX = 0;
    // for x
    for (int j = 0; j < boardSize; j++) {
      for (int i = 0; i < boardSize; i++) {
        if (getHexCellStatus(new Pos(j, i)) == cellStatus.c_x) {
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
        if (getHexCellStatus(new Pos(i, j)) == cellStatus.c_o) {
          scoreO++;
          break;
        }
      }
    }
  }

  boolean checkFinished(Player playerType) {
    boolean[][] visited = new boolean[boardSize][boardSize];
    Pos start = new Pos(0, 0);

    // loop through either x or y coordinate to check starting points are filled
    for (int i = 0; i < boardSize; ++i) {
      initializeVisited(visited);
      Stack path = new Stack();
      if (isConnected(start, visited, playerType, path)) {
        capitalizePath(path, playerType);
        return true;
      }

      // iterate correct coordinate
      switch (playerType.toString()) {
        case "x":
          start.setY(start.getY() + 1);
          break;
        case "o":
          start.setX(start.getX() + 1);
          break;
      }
    }
    return false;
  }

  void initializeVisited(boolean[][] visited) {
    // fill board
    for (int i = 0; i < boardSize; ++i) {
      for (int j = 0; j < boardSize; ++j) {
        visited[j][i] = false;
      }
    }
  }

  boolean isConnected(Pos p, boolean[][] visited, Player playerType, Stack path) {
    if (visited[p.getX()][p.getY()])
      return false;
    visited[p.getX()][p.getY()] = true;

    if (!hexCells[p.getX()][p.getY()].getStatus().toString().toLowerCase().equals(playerType.toString()))
      return false;

    int border = 0; // left side for x, bottom side for o
    switch (playerType.toString()) {
      case "x":
        border = p.getX();
        break;
      case "o":
        border = p.getY();
    }

    if (border == boardSize - 1 && hexCells[p.getX()][p.getY()].getStatus().toString().toLowerCase().equals(playerType.toString())) {
      path.push(p);
      return true;
    }

    ArrayList<Pos> neighbors = getNeighbors(p);
    boolean res = false;
    for (Pos neighbor : neighbors)
      if (checkInsideOfBoard(neighbor)) {
        res = res || isConnected(neighbor, visited, playerType, path);
      }

    if (res) {
      path.push(p);
    }

    return res;
  }

  public void capitalizePath(Stack path, Player playerType) {
    String c = playerType.toString().toUpperCase();
    cellStatus status;
    if (c.equals(cellStatus.c_X.toString())) {
      status = cellStatus.c_X;
    } else {
      status = cellStatus.c_O;
    }
    do {
      Pos p = path.pop();
      hexCells[p.getX()][p.getY()].setStatus(status);
    } while (!path.empty());
  }

  private int random(int end, int start) {
    Random rand = new Random();
    return start + rand.nextInt(end);
  }

  private int random(int end) {
    return random(end, 0);
  }

  public void smartMove() {
    // save state for undo:
    nextMoveHistory.push(nextMove);
    computerLastMoveHistory.push(computerLastMove);
    boolean played = false;

    switch (random(2)) {
      case 0:
        status = memoryStatus.defense;
        break;
      case 1:
        status = memoryStatus.attack;
    }

    if (status == memoryStatus.defense)
      played = blockOpponentsWay();
    if (!played && status == memoryStatus.attack)
      played = tryConnection();
    if (!played)
      played = tryNeighbors();
    if (!played)
      randomMove();
    turn = Player.playerX;
  }

  void updateOpponentPos() {
    for (int i = 0; i < boardSize; ++i) {
      for (int j = 0; j < boardSize; ++j) {
        if (hexCells[j][i].getStatus() == cellStatus.c_x) {
          Pos p = new Pos(j, i);
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
  boolean blockPossibleBridge(Pos lastMove) {
    char border = (lastMove.getX() < getBoardSize() / 2)
            ? 'l'
            : 'r'; // left or right (closest)
    boolean decided = false;
    Pos decision = new Pos();
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
      play(decision, Player.playerO);
      decision.print();
      computerLastMove.setX(decision.getX());
      computerLastMove.setY(decision.getY());
    }
    return decided;
  }

  boolean blockOpponentsWay() {
    updateOpponentPos();
    return blockPossibleBridge(getLastMove());
  }

  private Pos getLastMove() {
    return opponentPos.peek();
  }

  // tries to connect one side to other
  boolean tryConnection() {
    boolean played = false;
    int x = -1, y = 0;

    if (!isAvailable(nextMove.getX(), nextMove.getY()))
      return false;

    if (getHexCellStatus(nextMove.getX(), nextMove.getY()) == cellStatus.c_o ||
            getHexCellStatus(nextMove.getX(), nextMove.getY()) == cellStatus.empty) {
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
      play(new Pos(x, y), Player.playerO);
      computerLastMove.setX(x);
      computerLastMove.setY(y);
      new Pos(x, y).print();
      played = true;
      nextMove.setX(x);
      nextMove.setY(y);
    }
    return played;
  }

  boolean tryNeighbors() {
    ArrayList<Pos> neighbors = getNeighbors(new Pos(computerLastMove.getX(), computerLastMove.getY()));
    for (Pos neighbor : neighbors)
      if (isAvailable(neighbor.getX(), neighbor.getY())) {
        setHexCellStatus(neighbor, cellStatus.c_o);
        history.push(neighbor); // for undo
        neighbor.print();
        computerLastMove.set(neighbor);
        return true;
      }
    return false;
  }

  void randomMove() {
    ArrayList<Pos> arr = new ArrayList<>();
    for (int i = 0; i < boardSize; ++i)
      for (int j = 0; j < boardSize; ++j)
        if (getHexCellStatus(i, j) == cellStatus.empty)
          arr.add(new Pos(i, j));

    int r = random(arr.size());
    hexCells[arr.get(r).getX()][arr.get(r).getY()].setStatus(cellStatus.c_o);
    computerLastMove.set(arr.get(r));
    history.push(new Pos(arr.get(r).getX(), arr.get(r).getY())); // for undo
    arr.get(r).print();
  }

  memoryStatus getStatus() {
    return status;
  }

  Pos getComputerLastMove() {
    return computerLastMove;
  }

  Pos getNextMove() {
    return nextMove;
  }

  public int noMarked() {
    int marked = 0;
    for (int i = 0; i < boardSize; i++)
      for (int j = 0; j < boardSize; j++)
        if (hexCells[i][j].getStatus() != cellStatus.empty)
          marked++;

    return marked;
  }

  int noMarkedIn(ArrayList<Hex> games) {
    int marked = 0;
    for (Hex g : games)
      marked += g.noMarked();
    return marked;
  }

  public void undo() {
    if (history.empty()) {
      System.err.println("You are at the first step!");
      return;
    }
    Pos p = history.pop();
    setHexCellStatus(p, cellStatus.empty);
    turn = (turn == Player.playerX) ? Player.playerO : Player.playerX; // change turn
    if (vsComputer && turn.equals(Player.playerO)) {
      nextMove = nextMoveHistory.pop();
      computerLastMove = computerLastMoveHistory.pop();
    }
    if (isGameEnded()) {
      winner = "\0";
      decapitalizePath();
    }
  }

  private boolean isGameEnded() {
    return !winner.equals("\0");
  }

  public void decapitalizePath() {
    for (int i = 0; i < boardSize; i++)
      for (int j = 0; j < boardSize; j++) {
        if (hexCells[i][j].getStatus() == cellStatus.c_X)
          hexCells[i][j].setStatus(cellStatus.c_x);
        if (hexCells[i][j].getStatus() == cellStatus.c_O)
          hexCells[i][j].setStatus(cellStatus.c_o);
      }
  }

  public Stack getHistory() {
    return history;
  }

  Stack getComputerLastMoveHistory() {
    return computerLastMoveHistory;
  }

  Stack getNextMoveHistory() {
    return nextMoveHistory;
  }

  public Hex load(String filename) throws IOException {
    BufferedReader br = new BufferedReader(new FileReader(filename));
    FileIO f = new FileIO(br);
    Hex h = f.loadFile();
    br.close();
    return h;
  }

  public boolean save(String filename) {
    try {
      BufferedWriter bw = new BufferedWriter(new FileWriter(filename));
      FileIO f = new FileIO(bw);
      f.saveFile(this);
      bw.flush();
      bw.close();
      return true;
    } catch (IOException e) {
      e.printStackTrace();
    }
    return false;
  }

  public Stack getOpponentPos() {
    return opponentPos;
  }
}
