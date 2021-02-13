import java.util.ArrayList;

public class Stack {
  private ArrayList<Pos> cells;

  public Stack() {
    cells = new ArrayList<>();
  }

  public void push(Pos cell) {
    cells.add(cell);
  }


  Pos pop() {
    return cells.remove(cells.size()-1);
  }

  public boolean exist(Pos p) {
    return cells.contains(p);
  }

  Pos peek() {
    return cells.get(cells.size()-1);
  }

  boolean empty() {
    return cells.isEmpty();
  }


  public int size() {
    return cells.size();
  }
  public Pos get(int i) {
    return cells.get(i);
  }
}
