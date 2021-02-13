#include "HexArray1D.h"

namespace FatihKaanHex {
    HexArray1D::HexArray1D(bool vsComputer, int size) : AbstractHex(vsComputer, size) {
        initializeBoard();
    }

    HexArray1D::HexArray1D() : HexArray1D(isVsComputerInput(), boardSizeInput()) {}

    void HexArray1D::initializeBoard() {
        hexCells = (Cell *) malloc(boardSize * boardSize * sizeof(Cell));

        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                operator()(j, i)->setStatus(empty);
            }
        }
    }

    Cell *HexArray1D::operator()(int x, int y) const {
        AbstractHex::checkPos(x, y);
        return hexCells + (boardSize * x + y);
    }

    void HexArray1D::setSize(int size) {
        hexCells = (Cell *) realloc(hexCells, size * size * sizeof(Cell));
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                operator()(j, i)->setStatus(empty);
            }
        }
        AbstractHex::setSize(size);
    }

    HexArray1D::~HexArray1D() {
        free(hexCells);
    }
}