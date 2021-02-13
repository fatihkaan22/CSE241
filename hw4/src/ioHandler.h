#ifndef UNTITLED1_IOHANDLER_H
#define UNTITLED1_IOHANDLER_H

#include <string>


class ioHandler {
public:
    enum signal {
        load, save, newGame, go, marked, compare, none, undo
    };

    // reads integer from cin between given boundaries
    static int inputInteger(int lowerBoundary, int upperBoundary = INT16_MAX);

    static int charToIndex(char c);

    static int charToInt(char c);

    static int strToInt(std::string s);

    static signal inputPos(std::string &pos);
};

#endif //UNTITLED1_IOHANDLER_H