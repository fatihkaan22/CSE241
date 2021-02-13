#ifndef UNTITLED1_STACK_H
#define UNTITLED1_STACK_H

#include "Pos.h"
#include <vector>

class Stack {
private:
    Pos* cells;
    int used;
    int capacity;
public:

    Stack();

    void push(const Pos &cell);

    Pos pop();

    bool exist(const Pos &p);

    Pos peek();

    bool empty();

    Pos* getArray() {
        return cells;
    }

    int size() const { return used; }
    
    Stack& operator=(const Stack& s);

    Stack (const Stack& s);
    
    ~Stack ();

};


#endif //UNTITLED1_STACK_H
