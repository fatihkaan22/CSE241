#ifndef HW5_CELL_H
#define HW5_CELL_H

#include <iostream>

namespace FatihKaanHex {
    enum cellStatus {
        empty = '.', c_x = 'x', c_o = 'o', c_X = 'X', c_O = 'O'
    };

    class Cell {

    private:
        int x, y;
        cellStatus status;

    public:
        Cell(int x, int y, cellStatus status);

        Cell(int x, int y);

        Cell();

        int getX() const;

        int getY() const;

        cellStatus getStatus() const;

        void setX(int x);

        void setY(int y);

        void setStatus(cellStatus status);

        void set(int x, int y);

    };
}
#endif //HW5_CELL_H