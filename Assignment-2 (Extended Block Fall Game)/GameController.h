#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H
#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file);// Function that implements the gameplay
    void gameOver(string message,BlockFall &game);
    void recursiveDropGrid(int row_index,int column_index, BlockFall &game);
    void recursiveDropBlock(int columnEnding_index,int rowEnding_index,BlockFall &game);
    bool powerUpDetector(BlockFall &game);
    void rowCheck(BlockFall &game, bool gravity);
    void beforeCleaning(BlockFall &game, bool gravity);
    };


#endif //PA2_GAMECONTROLLER_H
