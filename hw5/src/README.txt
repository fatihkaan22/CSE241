# How to play?

- input commands when game ask for position:
x's Turn:
## available commands
- new : creates new game.
- go <gameId> : opens desired game according to specified id.

In main function I have tested:
- exceptions
- isGameValid() method
- operator==()
- read/write file
- reset, lastMove, numberOfMoves methods
- array w/ different types of games

I didn't understand the 'valid sequence of moves' part so I implemented 2 functions.

isGamesValid(..): Checks if boardSize < 5, number of x or o's are matches, is the turn correct. (for all given games)

isMovesValid(..): Checks if given games could create a valid game sequence.
                  Compares each game cells with previous game cells and checks if there is only one different cell.
