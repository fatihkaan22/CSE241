#include <iostream>
#include "ioHandler.h"


int ioHandler::inputInteger(int lowerBoundary, int upperBoundary) {
    int input;
    do {
        std::cin >> input;
        if (input < lowerBoundary || input > upperBoundary)
            std::cout << "Not valid, renter: ";

    } while (input < lowerBoundary || input > upperBoundary);
    return input;
}

// returns integer equivalent of alphabetic character (a -> 0, b ->1, .. etc.)
int ioHandler::charToIndex(char c) {
    if (c >= 'a' && c <= 'z') { // to make it case insensitive
        return static_cast<int>(c) - 'a';
    } else if (c >= 'A' && c <= 'Z') {
        return static_cast<int>(c) - 'A';
    } else {
        std::cerr << "Not an alphabetical character." << std::endl;
        return -1;
    }
}

// returns integer equivalent of numeric characters
int ioHandler::charToInt(char c) {
    return c - '0';
}

int ioHandler::strToInt(std::string s) {
    int res = 0;
    for (int i = s.length() - 1, k = 1; i >= 0; --i, k *= 10)
        res += charToInt(s[i]) * k;
    return res;
}

// return 0: if position, 1: if load, 2 if save
ioHandler::signal ioHandler::inputPos(std::string &pos) {
    std::string s;
    std::cin >> s;

    if (s == "load" || s == "LOAD") return load;
    else if (s == "save" || s == "SAVE") return save;
    else if (s == "new" || s == "NEW") return newGame;
    else if (s == "go" || s == "GO") return go;
    else if (s == "marked" || s == "MARKED") return marked;
    else if (s == "compare" || s == "COMPARE") return compare;
    else if (s.length() == 1) { // whitespace check
        std::string y;
        std::cin >> y;
        pos = s + y;
    } else {
        pos = s;
    }
    return none;
}
