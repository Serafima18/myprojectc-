#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "board.h"

class GameLogic {
public:
    GameLogic();
    void processTurn(int x, int y);

private:
    Board board;
    Color currentPlayer;
    bool isMoveValid(int fromX, int fromY, int toX, int toY);
    void switchPlayer();
};

#endif
