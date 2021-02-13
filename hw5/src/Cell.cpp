#include "Cell.h"

namespace FatihKaanHex {

    Cell::Cell(int x, int y) : x(x), y(y), status(empty) {}

    Cell::Cell(int x, int y, cellStatus status) : x(x), y(y), status(status) {}

    Cell::Cell() : x(0), y(0), status(empty) {}

    int Cell::getX() const { return x; }

    int Cell::getY() const { return y; }

    void Cell::setX(int x) { Cell::x = x; }

    void Cell::setY(int y) { Cell::y = y; }

    void Cell::set(int x, int y) {
        Cell::x = x;
        Cell::y = y;
    }

    cellStatus Cell::getStatus() const { return status; }

    void Cell::setStatus(cellStatus cellStatus) { status = cellStatus; }

    std::ostream &operator<<(std::ostream &out, const Cell &h) {
        char c;
        switch (h.getStatus()) {
            case c_x:
                c = 'x';
                break;
            case c_X:
                c = 'X';
                break;
            case c_o:
                c = 'o';
                break;
            case c_O:
                c = 'O';
                break;
            case empty:
                c = '.';
                break;
        }
        out << c;
        return out;
    }

}