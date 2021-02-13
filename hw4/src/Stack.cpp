#include "Stack.h"

void Stack::push(const Pos &cell) {
  if (used == capacity - 1) { // create new array with doubled in capacity
    int newCapacity = 2 * capacity;
    Pos *newArr = new Pos[newCapacity];
    for (size_t i = 0; i < capacity; i++) // array copy
      newArr[i] = cells[i];
    delete[] cells;
    cells = newArr;
    capacity = newCapacity;
  }
  cells[used] = cell;
  used++;
}

Pos Stack::pop() {
  Pos p = cells[used - 1];
  used--;
  if (used < capacity / 2 && capacity > 10) {
    int newCapacity = capacity / 2;
    Pos *newArr = new Pos[newCapacity];
    for (size_t i = 0; i < used; i++)
      newArr[i] = cells[i];
    capacity = newCapacity;
    delete[] cells;
    this->cells = newArr;
  }
  return p;
}

bool Stack::exist(const Pos &p) {
  for (int i = 0; i < used; i++)
    if (cells[i] == p)
      return true;
  return false;
}

Pos Stack::peek() { return cells[used - 1]; }

bool Stack::empty() { return (used == 0); }

Stack::Stack() {
  capacity = 10;
  used = 0;
  cells = new Pos[capacity];
}

Stack::Stack(const Stack &s) : capacity(s.capacity), used(s.used) {
  cells = new Pos[capacity];
  for (size_t i = 0; i < used; i++)
    cells[i] = s.cells[i];
}

Stack &Stack::operator=(const Stack &s) {
  if (this == &s) {
    return *this;
  }
  if (capacity < s.capacity) {
    capacity = s.capacity;
    delete[] cells;
    cells = new Pos[capacity];
  }
  used = s.used;
  for (size_t i = 0; i < used; i++) {
    cells[i] = s.cells[i];
  }
  return *this;
}

Stack::~Stack() { delete[] cells; }