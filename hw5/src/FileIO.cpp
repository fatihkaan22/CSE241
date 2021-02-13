#include "FileIO.h"
#include "Cell.h"
#include "AbstractHex.h"
#include <iostream>

namespace FatihKaanHex {
    using namespace std;

    string FileIO::getDataFromLine(const string &line) {
        return line.substr(line.find(':') + 1);
    }

// Overloaded load functions for different data types
    int FileIO::loadIntData(istream &inStream) {
        string line;
        getline(inStream, line);
        string strData = getDataFromLine(line);
        return stoi(strData);
    }

    void FileIO::loadData(string &data, istream &inStream) {
        string line;
        getline(inStream, line);
        data = getDataFromLine(line);
    }

    void FileIO::loadData(player &data, istream &inStream) {
        string line;
        getline(inStream, line);
        char charData = getDataFromLine(line)[0];
        data = (charData == 'x') ? playerX : playerO;
    }

    memoryStatus FileIO::loadMemStatusData(istream &inStream) {
        string line;
        getline(inStream, line);
        char charData = getDataFromLine(line)[0];
        return (charData == '0') ? memoryStatus::defense : memoryStatus::attack;
    }

    void FileIO::loadData(bool &data, istream &inStream) {
        string line;
        getline(inStream, line);
        char charData = getDataFromLine(line)[0];
        data = (charData == '1');
    }

    Stack FileIO::loadStackData(int size, istream &inStream) {
        Stack data;
        string line;
        getline(inStream, line);
        string strData = getDataFromLine(line);

        for (int i = 0; i < size; ++i) {
            int strIndex1 = strData.find(',');
            int strIndex2 = strData.find(' ');
            Pos p;
            p.setX(stoi(strData.substr(0, strIndex1)));
            p.setY(stoi(strData.substr(strIndex1 + 1, strIndex2)));
            data.push(p);
            strData = strData.substr(strIndex2 + 1);
        }
        return data;
    }


    string FileIO::stackToStr(Stack stack) {
        string s;
        for (int i = 0; i < stack.size(); ++i) {
            s += to_string(stack.getVector()[i].getX());
            s += ',';
            s += to_string(stack.getVector()[i].getY());
            s += ' ';
        }
        return s;
    }

// Overloaded saving functions for different data types
    void FileIO::saveData(const string &identifier, const int &data,
                          std::ostream &outStream) {
        outStream << identifier << ':' << data << endl;
    }

    void FileIO::saveData(const string &identifier, const string &data,
                          std::ostream &outStream) {
        outStream << identifier << ':' << data << endl;
    }

    void FileIO::saveData(const string &identifier, const bool &data,
                          std::ostream &outStream) {
        outStream << identifier << ':' << data << endl;
    }

    void FileIO::saveData(const string &identifier, const char &data,
                          std::ostream &outStream) {
        outStream << identifier << ':' << data << endl;
    }

}