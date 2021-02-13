#ifndef STACK_H
#define STACK_H

#include "gameStructures.h"

const int CAPACITY = (12 * 12) / 2;

typedef struct stack {
    int top = 0;
    pos array[CAPACITY];
} stack;

bool push(stack *s, pos p) {
    if (s->top >= CAPACITY)
        return false;
    s->array[s->top] = p;
    (s->top)++;
    return true;
}

pos pop(stack *s) {
    pos p;
    if (s->top <= 0) {
        std::cerr << "Stack is empty." << std::endl;
        p.x = p.y = -1;
        return p;
    }
    (s->top)--;
    return s->array[s->top];
}

bool exist(stack *s, pos p) {
    for (int i = 0; i < s->top; ++i)
        if (s->array[i].x == p.x && s->array[i].y == p.y)
            return true;
    return false;
}

pos peek(stack s) {
    return s.array[s.top - 1];
}

#endif