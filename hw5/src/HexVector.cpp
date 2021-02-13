#include "HexVector.h"
#include "Cell.h"
#include <iostream>
#include <random>

using namespace std;

namespace FatihKaanHex {

    HexVector::HexVector(bool vsComputer, int size)
            : AbstractHex(vsComputer, size) {
        initializeBoard();
    }

    HexVector::HexVector() : HexVector(isVsComputerInput(), boardSizeInput()) {
        /* Intentionally empty*/
    }

    void HexVector::initializeBoard() {
        for (int i = 0; i < boardSize; ++i) {
            vector<Cell *> row;
            for (int j = 0; j < boardSize; ++j)
                row.push_back(new Cell(j, i));
            hexCells.push_back(row);
        }
    }

    Cell *HexVector::operator()(int x, int y) const {
        AbstractHex::checkPos(x, y);
        return hexCells[x][y];
    }

    void HexVector::setSize(int size) {
        AbstractHex::setSize(size);
    }

}