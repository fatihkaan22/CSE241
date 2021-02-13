import java.util.Objects;


enum memoryStatus {defense, attack}

public class Pos {
  private int x, y;

  public Pos(int x, int y) {
    this.x = x;
    this.y = y;
  }

  public Pos() {
    this(0,0);
  }

  int getX() {
    return x;
  }

  int getY() {
    return y;
  }

  void setX(int x) {
    this.x = x;
  }

  void setY(int y) {
    this.y = y;
  }

  void set(Pos p) {
    x = p.x;
    y = p.y;
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (!(o instanceof Pos)) return false;
    Pos pos = (Pos) o;
    return x == pos.x && y == pos.y;
  }

  @Override
  public int hashCode() {
    return Objects.hash(x, y);
  }

  void print() {
    System.out.println(this);
//    char c = (char) ('a' + x);
//    System.out.print(c + y + 1 + '\n');
  }

  @Override
  public String toString() {
    return "Pos{" +
            "x=" + x +
            ", y=" + y +
            '}';
  }
}
