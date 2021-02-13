import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

public class HexGui {
  private Hex hex;
  private HexButton[][] buttons;
  private static final int MAX_BOARD_SIZE = 20;
  private JFrame f = new JFrame();
  private JPanel topPanel;
  private JPanel bottomPanel;
  private JPanel mainPanel;
  private JPanel sizePanel;
  private JPanel boardPanel;


  public HexGui() {
    this.hex = new Hex(false, 5);
    this.buttons = new HexButton[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    topPanel = new JPanel();
    bottomPanel = new JPanel();
    mainPanel = new JPanel();
    sizePanel = new JPanel();
    boardPanel = new JPanel();
  }

  public void drawBoard() {
    boardPanel.removeAll();
    boardPanel.setLayout(new GridLayout(0, (hex.boardSize * 2) - 1, 2, 2));
    SwingUtilities.updateComponentTreeUI(f);
    for (int i = 0; i < hex.getBoardSize(); i++) {
      addSpace(boardPanel, i);
      for (int j = 0; j < hex.getBoardSize(); j++) {
        HexButton b = new HexButton(j, i);
        buttons[j][i] = b;
        b.addActionListener(new ActionListener() {
          @Override
          public void actionPerformed(ActionEvent actionEvent) {
            if (!b.getText().equals(HexButton.dot)) return;
            if (!hex.isGameFinished()) {
              b.mark(hex.getTurn());
              boolean played = hex.play(new Pos(b.x, b.y), hex.turn);
              if (!played) throw new UnknownError("Not played");
              if (hex.vsComputer) {
                hex.smartMove();
                buttons[hex.computerLastMove.getX()][hex.computerLastMove.getY()].mark(Player.playerO);
              }
              if (hex.isGameFinished()) {
                System.out.println("Game end");
                updateButtons();
                JOptionPane.showMessageDialog(f, "Game Over");
              }
            }
          }

        });
        boardPanel.add(b);
      }
      addSpace(boardPanel, hex.getBoardSize() - i - 1);
    }

  }

  public void init() {
    mainPanel.setLayout(new BorderLayout());

    sizePanel.add(new JLabel("Board size:"));
    sizePanel.setLayout(new GridLayout(0, 5));
    JTextField sizeInput = new JTextField(4);
    sizePanel.add(sizeInput, BorderLayout.WEST);

    //top
    topPanel.add(sizePanel);
    topPanel.setLayout(new GridLayout(4, 1));
    ButtonGroup bg = new ButtonGroup();
    JRadioButton vsUser = new JRadioButton("User vs user");
    vsUser.setSelected(true);
    JRadioButton vsComputerRadio = new JRadioButton("User vs computer");
    bg.add(vsComputerRadio);
    bg.add(vsUser);
    topPanel.add(vsUser);
    topPanel.add(vsComputerRadio);
    JButton startButton = new JButton("Start");
    topPanel.add(startButton);
    startButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent actionEvent) {
        int boardSize = 0;
        try {
          boardSize = Integer.parseInt(sizeInput.getText());
        } catch (Exception e) {
          JOptionPane.showMessageDialog(f, "Board size input is not a number");
          return;
        }
        if (boardSize < 5 || boardSize > MAX_BOARD_SIZE) {
          JOptionPane.showMessageDialog(f, "Board size input must be greater than 5 and less than " + MAX_BOARD_SIZE);
          return;
        }
        boolean vsComputer = vsComputerRadio.isSelected();

        HexGui.this.hex = new Hex(vsComputer, boardSize);
        drawBoard();

      }
    });

    mainPanel.add(boardPanel);
    mainPanel.add(bottomPanel, BorderLayout.SOUTH);
    mainPanel.add(topPanel, BorderLayout.NORTH);

    topPanel.setBorder(BorderFactory.createEmptyBorder(30, 30, 0, 30));
    boardPanel.setBorder(BorderFactory.createEmptyBorder(30, 30, 30, 30));
    f.add(mainPanel, BorderLayout.CENTER);
    f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    f.setTitle("HexGame");
    f.setSize(900, 700);
    f.setLocationRelativeTo(null); // center
//    p.setSize(500, 500);
    boardPanel.setVisible(true);
    f.setVisible(true);

    JButton loadButton = new JButton("Load");
    JButton saveButton = new JButton("Save");
    JButton undoButton = new JButton("Undo");
    bottomPanel.add(undoButton);
    bottomPanel.add(loadButton);
    bottomPanel.add(saveButton);

    undoButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent actionEvent) {
        if (hex.history.empty()) {
          JOptionPane.showMessageDialog(f,"Cannot undo");
          return;
        }
        hex.undo();
        if (hex.vsComputer) hex.undo();
        drawBoard();
        updateButtons();
      }
    });

    saveButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent actionEvent) {
        String file = JOptionPane.showInputDialog("Enter file name");
        if (file != null) {
          hex.save(file);
        }
      }
    });

    loadButton.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent actionEvent) {
        String file = JOptionPane.showInputDialog("Enter file name");
        if (file != null) {
          try {
            HexGui.this.hex = hex.load(file);
            drawBoard();
            updateButtons();
          } catch (IOException e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(f, "File cannot be loaded.");
          }
        }
      }
    });

  }


  private void updateButtons() {
    for (int i = 0; i < hex.getBoardSize(); i++) {
      for (int j = 0; j < hex.getBoardSize(); j++) {
        buttons[j][i].setText(hex.getHexCellStatus(j, i).toString());
//        if (hex.getHexCellStatus(j, i).toString().equals(".") || buttons[j][i].getText().equals(HexButton.dot)) {
        if (hex.getHexCellStatus(j, i).toString().equals(".")) {
          buttons[j][i].setBackground(Color.white);
          buttons[j][i].setText(HexButton.dot);
        } else {
          buttons[j][i].setForeground(Color.white);
        }

        if (hex.getHexCellStatus(j, i).toString().equals("x"))
          buttons[j][i].setBackground(HexButton.red);
        if (hex.getHexCellStatus(j, i).toString().equals("o"))
          buttons[j][i].setBackground(HexButton.blue);
        // mark the winning path
        if (hex.getHexCellStatus(j, i).toString().equals("X"))
          buttons[j][i].setBackground(HexButton.darkRed);
        if (hex.getHexCellStatus(j, i).toString().equals("O"))
          buttons[j][i].setBackground(HexButton.darkBlue);
      }
    }
  }


  private void addSpace(JPanel p, int i) {
    for (int j = 0; j < i; j++) {
      JLabel l = new JLabel(" ");
      l.setSize(10, 10);
      p.add(l);
    }
  }
}


