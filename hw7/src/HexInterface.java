import java.io.IOException;

public interface HexInterface {
  int MIN_SIZE = 5; //static final

  int getBoardSize();

  boolean play(Pos pos, Player p);

  boolean play(String pos, Player p);

  boolean isGameFinished();

  void smartMove();

  void undo();

  Hex load(String filename) throws IOException;

  boolean save(String filename);

  boolean checkInsideOfBoard(Pos p);

  boolean isAvailable(int x, int y);

  void capitalizePath(Stack path, Player playerType);

  int noMarked();

  void decapitalizePath();

  Stack getHistory();
}
