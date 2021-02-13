#ifndef HW5_HEXVECTOR_H
#define HW5_HEXVECTOR_H

#include <string>
#include "AbstractHex.h"
#include "Cell.h"

namespace FatihKaanHex {

    class HexVector : public AbstractHex {
    private:

        std::vector<std::vector<Cell *> > hexCells;

        void initializeBoard();

    public:
        HexVector(bool vsComputer, int boardSize);

        void setSize(int size) override;

        Cell *operator()(int x, int y) const override;

        HexVector();
    };
}
#endif //HW5_HEXVECTOR_H
