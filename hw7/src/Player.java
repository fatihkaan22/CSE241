enum Player {
  playerO("o"), playerX("x");
  private String label;

  private Player(String l) {
    this.label = l;
  }

  @Override
  public String toString() {
    return label;
  }
}
