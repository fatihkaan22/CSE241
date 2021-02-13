#ifndef UNTITLED1_POS_H
#define UNTITLED1_POS_H

class Pos {
private:
  int x;
  int y;

public:
  Pos(int x, int y);

  int getX() const;

  int getY() const;

  void set(const Pos &p);

  bool operator==(const Pos &p) const;

  Pos &operator=(const Pos &p);

  void setX(int x);

  void setY(int y);

  void print() const;

  Pos();

};

#endif // UNTITLED1_POS_H
