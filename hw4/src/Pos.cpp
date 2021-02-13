#include "Pos.h"
#include <iostream>

Pos::Pos(int x, int y) : x(x), y(y) {}

Pos::Pos() : x(0), y(0) {}

int Pos::getX() const { return x; }

int Pos::getY() const { return y; }

void Pos::setX(int x) { Pos::x = x; }

void Pos::setY(int y) { Pos::y = y; }

void Pos::set(const Pos &p) {
  x = p.x;
  y = p.y;
}

bool Pos::operator==(const Pos &p) const { return x == p.x && y == p.y; }

Pos& Pos::operator=(const Pos &p) {
  x = p.x;
  y = p.y;
  return *this;
}

void Pos::print() const {
  char c = 'a' + x;
  std::cout << c << y + 1 << std::endl;
}