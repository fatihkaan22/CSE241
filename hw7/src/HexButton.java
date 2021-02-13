import javax.swing.*;
import javax.swing.border.CompoundBorder;
import javax.swing.border.LineBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class HexButton extends JButton implements ActionListener {
  public static final Color red = new Color(254, 67, 60);
  public static final Color darkRed = new Color(204, 0, 102);
  public static final Color darkBlue = new Color(0, 255, 255);
  public static final Color blue = new Color(0, 149, 239);
  public static final String dot = "·";
  int x, y;

  public void mark(Player p) {

    setForeground(Color.WHITE);
    if (p.equals(Player.playerX)) {
      setBackground(HexButton.red);
      setText("x");
    } else if (p.equals(Player.playerO)) {
      setBackground(HexButton.blue);
      setText("o");
    }
  }

  public HexButton(int x, int y) {
    this.x = x;
    this.y = y;
    setText(dot);
    setBackground(Color.white);
    this.setBorder(new LineBorder(Color.BLACK));
  }

  @Override
  public void actionPerformed(ActionEvent actionEvent) {
  }

}
