#ifndef HW5_HEXARRAY1D_H
#define HW5_HEXARRAY1D_H

#include "Cell.h"
#include "AbstractHex.h"
#include "Stack.h"

namespace FatihKaanHex {
    class HexArray1D : public AbstractHex {
    private:

        HexArray1D();

        Cell *hexCells;

        void initializeBoard();

    public:
        void setSize(int size) override;

        Cell *operator()(int x, int y) const override;

        HexArray1D(bool vsComputer, int size);

        ~HexArray1D();
    };

}
#endif //HW5_HEXARRAY1D_H
