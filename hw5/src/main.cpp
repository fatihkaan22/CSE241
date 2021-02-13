#include <iostream>
#include <stack>
#include <fstream>
#include "HexVector.h"
#include "HexAdapter.h"
#include "HexArray1D.h"

using namespace std;
using namespace FatihKaanHex;

bool isMovesValid(AbstractHex *games[], int arrSize);
bool isGameValid(AbstractHex *game);
bool isGamesValid(AbstractHex *games[], int arrSize);
void testException();
void testIsGameValid();
void testGameEqual() ;
void testWriteReadFile() ; // load game0 to game1
void testReset() ;
void testLastMove() ;
void testNumberOfMoves() ;

int main() {
    cout << "---------testException()---------" << endl;
    testException();
    cout << "---------testIsGameValid()---------" << endl;
    testIsGameValid();
    cout << "---------testGameEqual()---------" << endl;
    testGameEqual();
    cout << "---------testWriteReadFile()---------" << endl;
    testWriteReadFile();
    cout << "---------testReset()---------" << endl;
    testReset();
    cout << "---------testLastMove()---------" << endl;
    testLastMove();
    cout << "---------testNumberOfMoves()---------" << endl;
    testNumberOfMoves();


    // different types of games
    HexVector game0(false, 5);
    HexArray1D game1(true, 7);
    HexAdapter<std::vector> game2(true, 5);
    HexAdapter<std::deque> game3(false, 8);

    vector<AbstractHex *> games;
    games.push_back(&game0);
    games.push_back(&game1);
    games.push_back(&game2);
    games.push_back(&game3);


    AbstractHex *gamesArr[] = {&game0, &game1, &game2, &game3};

    // usual game loop
    AbstractHex *currentGame = games[0];
    AbstractHex::activeGameId = games[0]->getGameId();
    while (true) {
        cout << "\nCurrent game: " << currentGame->getGameId() << endl;
        cout << "Active games: [ ";
        for (auto i : games)
            cout << i->getGameId() << " | ";
        cout << "\b\b]\n";

        if (currentGame->getGameId() == AbstractHex::activeGameId) {
            currentGame->playGame();
        } else {
            if (AbstractHex::activeGameId >= games.size()) {
                games.push_back(new HexVector());  // it could be any game
            }
            currentGame = games[AbstractHex::activeGameId];
        }
    }

}


bool isMovesValid(AbstractHex *games[], int arrSize) {
    for (int i = 0; i < arrSize - 1; ++i) {
        if (games[i + 1]->getBoardSize() != games[i]->getBoardSize())
            return false;
        if (games[i + 1]->numberOfMoves() - games[i]->numberOfMoves() != 1)
            return false;
        // if there are 2 different cell positions then return false
        int noDiff = 0;
        for (int k = 0; k < games[i]->getBoardSize(); ++k) {
            for (int j = 0; j < games[i]->getBoardSize(); ++j) {
                if (games[i]->operator()(j, k)->getStatus() != games[i + 1]->operator()(j, k)->getStatus())
                    ++noDiff;
                if (noDiff > 1) return false;
            }
        }
    }
    return true;
}

// checks if the given board valid
bool isGameValid(AbstractHex *game) {
    // check board size
    if (game->getBoardSize() < 5)
        return false;
    // are number of x or o's equal or diff is one
    int noX, noO; //number of x, number of o
    noX = noO = 0;
    for (int i = 0; i < game->getBoardSize(); ++i) {
        for (int j = 0; j < game->getBoardSize(); ++j) {
            if (game->operator()(i, j)->getStatus() == cellStatus::c_x) noX++;
            if (game->operator()(i, j)->getStatus() == cellStatus::c_o) noO++;
        }
    }
    if (noX - noO > 1 || noX - noO < -1)
        return false;
    // check if the turn is correct
    if (noX > noO)
        return game->getTurn() == playerO;
    if (noO > noX)
        return game->getTurn() == playerX;
    return true;
}

bool isGamesValid(AbstractHex *games[], int arrSize) {
    for (int i = 0; i < arrSize; ++i)
        if (!isGameValid(games[i]))
            return false;
    return true;
}

void testException() {
    HexVector game0(false, 5);
    // exception test
    try {
        // out of board index
        game0(0, 50);
    } catch (range_error &exception) {
        cout << "exception test 1: " << exception.what() << endl;
    }

    try {
        // there is no last move
        game0.lastMove().getX();
    } catch (runtime_error &exception) {
        cout << "exception test 2: " << exception.what() << endl;
    }
}

void testIsGameValid() {
    HexVector game0(false, 5);
    HexArray1D game1(true, 7);
    HexAdapter<std::vector> game2(true, 5);
    HexAdapter<std::deque> game3(false, 8);
    vector<AbstractHex *> games;

    games.push_back(&game0);
    games.push_back(&game1);
    games.push_back(&game2);
    games.push_back(&game3);

    AbstractHex *gamesArr[] = {&game0, &game1, &game2, &game3};

    cout << "isGameValid0: " << isGameValid(&game0) << endl; // valid
    cout << "isGameValid1: " << isGameValid(&game1) << endl; // valid
    cout << "isGameValid2: " << isGameValid(&game2) << endl; // valid
    cout << "isGameValid3: " << isGameValid(&game3) << endl; // valid
    game0.play("a3", playerX);
    game0.play("b3", playerX);
    // invalid moves since x played 2 times
    cout << "isGameValid4: " << isGameValid(&game0) << endl; // not valid
    game1.play("a1", playerX);
    game1.play(); // smart move
    //valid: player1, then computer move
    cout << "isGameValid5: " << isGameValid(&game1) << endl; // valid
}

void testGameEqual() {
    HexVector game0(false, 5);
    HexArray1D game1(false, 5);
    HexAdapter<std::vector> game2(false, 6);
    game0.play("d3", playerX);
    game1.play("d3", playerX);
    cout << "game0 == game1: " << (game0 == game1) << endl; // true
    game0.play("a3", playerX);
    cout << "game0 == game1: " << (game0 == game1) << endl; // false
    cout << "game0 == game2: " << (game0 == game1) << endl; // false (boardSize different)
}

void testWriteReadFile() { // load game0 to game1
    HexVector game0(false, 5);
    HexAdapter<std::vector> game1(false, 5);
    game0.play("d3", playerX);
    game0.play("e3", playerO);
    game0.play("a2", playerX);
    game0.play("b5", playerO);

    ofstream oStream;
    oStream.open("saveTest");
    game0.writeToFile(oStream);
    oStream.close();
    game0.print();
    cout << endl;

    ifstream inStream;
    inStream.open("saveTest");
    game1.readFromFile(inStream);
    inStream.close();
    game1.print();
}

void testReset() {
    HexVector game0(false, 5);
    game0.play("a3", playerX);
    game0.play("b3", playerO);
    game0.print();
    cout << endl;
    game0.reset();
    game0.print();
}

void testLastMove() {
    HexVector game0(false, 5);
    try {
        game0.lastMove().print();
    } catch (runtime_error e) {
        cout << "Intentional exception: " << e.what();
    }

    game0.play("a3", playerX);
    game0.play("b3", playerO);
    game0.lastMove().print();
}

void testNumberOfMoves() {
    HexAdapter<std::deque> game0(false, 8);
    cout << "noMoves: " << game0.numberOfMoves() << endl; // 0
    game0.play("a3", playerX);
    game0.play("b3", playerO);
    cout << "noMoves: " << game0.numberOfMoves() << endl; // 2
}
