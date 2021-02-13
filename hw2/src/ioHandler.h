#ifndef UNTITLED_IOHANDLER_H
#define UNTITLED_IOHANDLER_H

#include <iostream>
#include <string>
#include <fstream>
#include "gameStructures.h"

using namespace std;

string getDataFromLine(const string &line) {
    return line.substr(line.find(':') + 1);
}

// Overloaded load functions for different data types
void loadData(ifstream &inStream, int &data) {
    string line;
    getline(inStream, line);
    string strData = getDataFromLine(line);
    data = stoi(strData);
}

void loadData(ifstream &inStream, CELL data[MAX_SIZE][MAX_SIZE], const int &size) {
    string line;
    getline(inStream, line);
    string strData = getDataFromLine(line);
    int strIndex = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            CELL c = static_cast<CELL>(strData[strIndex++]);
            data[j][i] = c;
        }
    }
}

void loadData(ifstream &inStream, MARK &data) {
    string line;
    getline(inStream, line);
    char charData = getDataFromLine(line)[0];
    data = (charData == 'x') ? MARK::X : MARK::O;
}

void loadData(ifstream &inStream, STATUS &data) {
    string line;
    getline(inStream, line);
    char charData = getDataFromLine(line)[0];
    data = (charData == '0') ? STATUS::defense : STATUS::attack;
}

void loadData(ifstream &inStream, bool &data) {
    string line;
    getline(inStream, line);
    char charData = getDataFromLine(line)[0];
    data = (charData == '1') ? true : false;
}

void loadData(ifstream &inStream, pos data[], const int &size) {
    string line;
    getline(inStream, line);
    string strData = getDataFromLine(line);

    for (int i = 0; i < size; ++i) {
        int strIndex1 = strData.find(',');
        int strIndex2 = strData.find(' ');
        data[i].x = stoi(strData.substr(0, strIndex1));
        data[i].y = stoi(strData.substr(strIndex1 + 1, strIndex2));
        strData = strData.substr(strIndex2 + 1);
    }
}

// returns true if there is error on file opening
bool loadFile(const string &fileName, CELL board[MAX_SIZE][MAX_SIZE], int &boardSize, bool &vsComputer, MARK &nextTurn,
              memory &computerMemory) {
    ifstream inStream;
    inStream.open(fileName);
    if (inStream.fail()) {
        cerr << "File open failed\n";
        return true;
    }

    loadData(inStream, boardSize);
    loadData(inStream, board, boardSize);
    loadData(inStream, nextTurn);
    loadData(inStream, vsComputer);
    if (vsComputer) {
        loadData(inStream, computerMemory.opponentPos.top);
        loadData(inStream, computerMemory.opponentPos.array, computerMemory.opponentPos.top);
        loadData(inStream, computerMemory.computerLastMove.x);
        loadData(inStream, computerMemory.computerLastMove.y);
        loadData(inStream, computerMemory.nextMove.x);
        loadData(inStream, computerMemory.nextMove.y);
        loadData(inStream, computerMemory.status);
    }
    return false;
}


string boardToStr(const CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize) {
    string s;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            s += static_cast<char>(board[j][i]);
        }
    }
    return s;
}

string arrToStr(const pos arr[], const int &size) {
    string s;
    for (int i = 0; i < size; ++i) {
        s += to_string(arr[i].x);
        s += ',';
        s += to_string(arr[i].y);
        s += ' ';
    }

    return s;
}

// Overloaded saving functions for different data types
void saveData(const string &identifier, const int &data, ofstream &outStream) {
    outStream << identifier << ':' << data << endl;
}

void saveData(const string &identifier, const string &data, ofstream &outStream) {
    outStream << identifier << ':' << data << endl;
}

void saveData(const string &identifier, const bool &data, ofstream &outStream) {
    outStream << identifier << ':' << data << endl;
}

void saveData(const string &identifier, const char &data, ofstream &outStream) {
    outStream << identifier << ':' << data << endl;
}

// returns true if there is error on file opening
bool
saveFile(const string &fileName, const CELL board[MAX_SIZE][MAX_SIZE], const int &boardSize, const bool &vsComputer,
         const MARK &nextTurn, const memory &computerMemory) {
    ofstream outStream;
    outStream.open(fileName);
    if (outStream.fail()) {
        cerr << "File open failed\n";
        return true;
    }

    saveData("boardSize", boardSize, outStream);
    saveData("board", boardToStr(board, boardSize), outStream);
    saveData("nextTurn", static_cast<char>(nextTurn), outStream);
    saveData("vsComputer", vsComputer, outStream);

    if (vsComputer) {
        saveData("computerMemory.opponentPos.top", computerMemory.opponentPos.top, outStream);
        saveData("computerMemory.opponentPos.array",
                 arrToStr(computerMemory.opponentPos.array, computerMemory.opponentPos.top), outStream);
        saveData("computerMemory.computerLastMove.x", computerMemory.computerLastMove.x, outStream);
        saveData("computerMemory.computerLastMove.y", computerMemory.computerLastMove.y, outStream);
        saveData("computerMemory.nextMove.x", computerMemory.nextMove.x, outStream);
        saveData("computerMemory.nextMove.y", computerMemory.nextMove.y, outStream);
        saveData("computerMemory.status", computerMemory.status, outStream);
    }
    outStream.close();
    return false;
}

#endif //UNTITLED_IOHANDLER_H