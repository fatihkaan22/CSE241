# How to play?

- input commands when game ask for position:

x's Turn:

## available commands
- new : creates new game.
- open <gameId> : opens desired game according to specified id.
- marked : prints number of marked cells
- compare <gameId> : prints true if current game has more marked cells compare to game with specified id; false otherwise.

# main function

- Normally game starts with one game but in main function I have manually added 5 games since it is stated in HW paper.
- For an usual game example there is a function called 'usualGame()'

Note: load/save game functions will not work if you try to load a bigger board, to a smaller board, since allocated space of the object in the vector can not vary.