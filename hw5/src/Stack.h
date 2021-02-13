#ifndef HW5_STACK_H
#define HW5_STACK_H

#include "Pos.h"
#include <vector>

namespace FatihKaanHex {
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

        void print();
    };
}

#endif //HW5_STACK_H
