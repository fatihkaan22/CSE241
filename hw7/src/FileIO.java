import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;


public class FileIO {
  private BufferedWriter bw;
  private BufferedReader br;


  public FileIO(BufferedWriter bw) {
    this.bw = bw;
  }

  public FileIO(BufferedReader br) {
    this.br = br;
  }

  String getDataFromLine(String line) {
    return line.substring(line.indexOf(':') + 1);
  }

  // Overloaded load functions for different data types

  private int loadIntData() throws IOException {
    String strData = getDataFromLine(br.readLine());
    return Integer.parseInt(strData);
  }


  void loadData(Cell[][] data, int size) throws IOException {
    cellStatus cell;
    String line = br.readLine();
    String strData = getDataFromLine(line);
    int strIndex = 0;
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        char c = (strData.charAt(strIndex++));
        switch (c) {
          case 'X':
            cell = cellStatus.c_X;
            break;
          case 'O':
            cell = cellStatus.c_O;
            break;
          case 'x':
            cell = cellStatus.c_x;
            break;
          case 'o':
            cell = cellStatus.c_o;
            break;
          default:
            cell = cellStatus.empty;
            break;
        }
        data[j][i].setStatus(cell);
      }
    }
  }

  Player loadPlayerData() throws IOException {
    String line = br.readLine();
    char charData = getDataFromLine(line).charAt(0);
    return (charData == 'x') ? Player.playerX : Player.playerO;
  }

  memoryStatus loadMemStatusData() throws IOException {
    String line = br.readLine();
    String strData = getDataFromLine(line);
    return (strData.equals("defense")) ? memoryStatus.defense : memoryStatus.attack;
  }

  boolean loadBoolData() throws IOException {
    String line = br.readLine();
    String strData = getDataFromLine(line);
    return (strData.equals("true"));
  }

  Stack loadStackData(int size) throws IOException {
    Stack data = new Stack();
    String line = br.readLine();
    String strData = getDataFromLine(line);

    for (int i = 0; i < size; ++i) {
      int strIndex1 = strData.indexOf(',');
      int strIndex2 = strData.indexOf(' ');
      Pos p = new Pos();
      p.setX(Integer.parseInt(strData.substring(0, strIndex1)));
      p.setY(Integer.parseInt(strData.substring(strIndex1 + 1, strIndex2)));
      data.push(p);
      strData = strData.substring(strIndex2 + 1);
    }
    return data;
  }

  // returns true if there is error
  Hex loadFile() throws IOException {
    int boardSize = loadIntData();
    boolean vsComputer = loadBoolData();
    Hex hex = new Hex(vsComputer, boardSize);
    hex = new Hex(hex.vsComputer, hex.boardSize);
    loadData(hex.hexCells, hex.boardSize);
    hex.turn = loadPlayerData();
    int histSize = loadIntData();
    hex.history = loadStackData(histSize);
    if (hex.vsComputer) {
      int size = loadIntData();
      hex.opponentPos = loadStackData(size);
      int x = loadIntData();
      int y = loadIntData();
      hex.computerLastMove.setX(x);
      hex.computerLastMove.setY(y);
      x = loadIntData();
      y = loadIntData();
      hex.nextMove = new Pos(x, y);
      hex.status = loadMemStatusData();
      int compLastMoveHistSize = loadIntData();
      hex.computerLastMoveHistory = loadStackData(compLastMoveHistSize);
      int nextMoveHistSize = loadIntData();
      hex.nextMoveHistory = loadStackData(nextMoveHistSize);
    }
    return hex;
  }

  String boardToStr(Hex hex) {
    StringBuilder sb = new StringBuilder();
    for (int i = 0; i < hex.boardSize; ++i) {
      for (int j = 0; j < hex.boardSize; ++j) {
        sb.append(hex.hexCells[j][i].getStatus());
      }
    }
    return sb.toString();
  }

  String stackToStr(Stack stack) {
    StringBuilder sb = new StringBuilder();
    for (int i = 0; i < stack.size(); ++i) {
      sb.append(stack.get(i).getX()).append(',');
      sb.append(stack.get(i).getY()).append(' ');
    }
    return sb.toString();
  }

  void saveData(String identifier, String data) throws IOException {
    bw.write(identifier + ":" + data);
    bw.newLine();
  }


  void saveFile(Hex hex) throws IOException {
    saveData("boardSize", String.valueOf(hex.boardSize));
    saveData("vsComputer", String.valueOf(hex.vsComputer));
    saveData("board", boardToStr(hex));
    saveData("nextTurn", String.valueOf(hex.turn));
    saveData("history.size", String.valueOf(hex.history.size()));
    saveData("history", stackToStr(hex.history));

    if (hex.vsComputer) {
      saveData("computerMemory.opponentPos.top", String.valueOf(hex.getOpponentPos().size()));
      saveData("computerMemory.opponentPos.array", stackToStr(hex.getOpponentPos()));
      saveData("computerMemory.computerLastMove.x", String.valueOf(hex.getComputerLastMove().getX()));
      saveData("computerMemory.computerLastMove.y", String.valueOf(hex.getComputerLastMove().getY()));
      saveData("computerMemory.nextMove.x", String.valueOf(hex.getNextMove().getX()));
      saveData("computerMemory.nextMove.y", String.valueOf(hex.getNextMove().getX()));
      saveData("computerMemory.status", hex.getStatus().toString());
      saveData("computerLastMoveHistory.size", String.valueOf(hex.computerLastMoveHistory.size()));
      saveData("computerLastMoveHistory", stackToStr(hex.computerLastMoveHistory));
      saveData("nextMoveHistory.size", String.valueOf(hex.nextMoveHistory.size()));
      saveData("nextMoveHistory", stackToStr(hex.nextMoveHistory));
    }
  }
}
