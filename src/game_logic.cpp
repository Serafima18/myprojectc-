#include "game_logic.h"
#include <iostream>

GameLogic::GameLogic() : currentPlayer(Color::RED) {}

void GameLogic::processTurn(int x, int y) {
    // This is where you would handle clicks or moves
    std::cout << "Processing turn at (" << x << ", " << y << ")" << std::endl;

    // Example: switch player
    switchPlayer();
}

bool GameLogic::isMoveValid(int fromX, int fromY, int toX, int toY) {
    // Add logic here to validate moves
    return true;
}

void GameLogic::switchPlayer() {
    currentPlayer = (currentPlayer == Color::RED) ? Color::WHITE : Color::RED;
}
