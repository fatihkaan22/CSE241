#include "Hex.h"
#include <iostream>

using namespace std;

int main() {
  // TEST GAMES
  Hex test0(false, 7);
  Hex::games.push_back(&test0);
  Hex test1(true, 5);
  Hex::games.push_back(&test1);
  Hex test2(false, 8);
  Hex::games.push_back(&test2);
  Hex test3(true, 18);
  Hex::games.push_back(&test3);
  Hex test4(false, 9);
  Hex::games.push_back(&test4);
  Hex::activeGameId = 0; // since active game don't be set naturally
  Hex *currentGame = Hex::games[0];
  
  // insertion operation test
  // cout << test0 << endl;
  
  int lastGameId = 0;
  while (true) {
    if (lastGameId == Hex::activeGameId) {
      currentGame->playGame();
    } else {
      lastGameId = Hex::activeGameId;
      if (Hex::activeGameId >= Hex::games.size()) {
        Hex::games.push_back(new Hex());
      }
      currentGame = Hex::games[Hex::activeGameId];
    }
  }

  return 0;
}

int usualGame() {
  Hex first;
  Hex::games.push_back(&first);
  Hex *currentGame = Hex::games[0];
  Hex::activeGameId = 0;

  int lastGameId = 0;
  while (true) {
    if (lastGameId == Hex::activeGameId) {
      currentGame->playGame();
    } else {
      lastGameId = Hex::activeGameId;
      if (Hex::activeGameId >= Hex::games.size()) {
        Hex::games.push_back(new Hex());
      }
      currentGame = Hex::games[Hex::activeGameId];
    }
  }
}
