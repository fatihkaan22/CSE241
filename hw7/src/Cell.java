enum cellStatus {
  empty("."), c_x("x"), c_o("o"), c_X("X"), c_O("O");

  private String l;

  private cellStatus(String l) {
    this.l = l;
  }

  @Override
  public String toString() {
    return l;
  }
}

public class Cell {
  private int x, y;

  @Override
  public String toString() {
    return "Cell{" +
            "x=" + x +
            ", y=" + y +
            ", status=" + status +
            '}';
  }

  private cellStatus status;

  public Cell(int x, int y, cellStatus status) {
    this.x = x;
    this.y = y;
    this.status = status;
  }

  public Cell(int x, int y) {
    this(x, y, cellStatus.empty);
  }

  public Cell() {
    this(0, 0, cellStatus.empty);
  }


  public cellStatus getStatus() {
    return status;
  }

  public void set(int x, int y) {
    this.x = x;
    this.y = y;
  }

  public void setStatus(cellStatus cellStatus) {
    status = cellStatus;
  }


}
