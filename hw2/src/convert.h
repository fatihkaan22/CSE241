int charToIndex(const char &c);

int charToInt(const char &c);

int strToInt(const std::string &s);

// returns integer equivalent of alphabetic character (a -> 0, b ->1, .. etc.)
int charToIndex(const char &c) {
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
int charToInt(const char &c) {
    return c - '0';
}

int strToInt(const std::string &s) {
    int res = 0;
    for (int i = s.length() - 1, k = 1; i >= 0; --i, k *= 10)
        res += charToInt(s[i]) * k;
    return res;
}
