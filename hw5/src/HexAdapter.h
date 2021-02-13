// I didn't separate declaration and implementation for this templated class.
// If we separate it, client code needs to include 'HexAdapter.cpp', which is not a good practice.

#ifndef HW5_HEXADAPTER_H
#define HW5_HEXADAPTER_H

#include "AbstractHex.h"

namespace FatihKaanHex {

    template<template<class T, class Allocator = std::allocator<T> > class T>
    class HexAdapter : public AbstractHex {
    public:
        HexAdapter(bool vsComputer, int size);

        HexAdapter();

        void setSize(int size) override;

        Cell *operator()(int x, int y) const override;

    private:
        T<T<Cell * >> hexCells;

        void initializeBoard();

    };


    template<template<class T, class Allocator = std::allocator<T> > class T>
    Cell *HexAdapter<T>::operator()(int x, int y) const {
        AbstractHex::checkPos(x, y);
        return hexCells[x][y];
    }

    template<template<class T, class Allocator = std::allocator<T> > class T>
    void HexAdapter<T>::setSize(int size) {
        AbstractHex::setSize(size);
    }

    template<template<class T, class Allocator = std::allocator<T> > class T>
    HexAdapter<T>::HexAdapter(bool vsComputer, int size) : AbstractHex(vsComputer, size) {
        initializeBoard();
    }

    template<template<class T, class Allocator = std::allocator<T> > class T>
    void HexAdapter<T>::initializeBoard() {
        for (int i = 0; i < boardSize; ++i) {
            T<Cell *> row;
            for (int j = 0; j < boardSize; ++j)
                row.push_back(new Cell(i, j));
            hexCells.push_back(row);
        }

    }

    template<template<class T, class Allocator = std::allocator<T> > class T>
    HexAdapter<T>::HexAdapter() : HexAdapter<T>(isVsComputerInput(), boardSizeInput()) {
        /* Intentionally empty */
    }

}
#endif //HW5_HEXADAPTER_H
