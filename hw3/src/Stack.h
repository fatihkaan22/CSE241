#ifndef UNTITLED1_STACK_H
#define UNTITLED1_STACK_H

#include "Pos.h"
#include <vector>

class Stack {
private:
    std::vector<Pos> cells;
public:

    Stack();

    void push(const Pos &cell);

    Pos pop();

    bool exist(const Pos &p);

    Pos peek();

    bool empty();

    std::vector<Pos> getVector() {
        return cells;
    }

    int size() const { return cells.size(); }

};


#endif //UNTITLED1_STACK_H
