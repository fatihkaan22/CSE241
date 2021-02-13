#ifndef HW5_IOHANDLER_H
#define HW5_IOHANDLER_H

#include <string>
#include "AbstractHex.h"

namespace FatihKaanHex {
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

        static void handleSignal(signal s, AbstractHex *hex);
    };

}
#endif //HW5_IOHANDLER_H
