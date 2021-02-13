#include "Stack.h"

void Stack::push(const Pos &cell) {
    cells.push_back(cell);
}

Pos Stack::pop() {
    Pos p = cells.back();
    cells.pop_back();
    return p;
}

bool Stack::exist(const Pos &p) {
    for (auto &cell : cells)
        if (cell == p)
            return true;
    return false;
}

Pos Stack::peek() {
    return cells.back();
}

bool Stack::empty() {
    return (cells.empty());
}

Stack::Stack() : cells(std::vector<Pos>()) {}
