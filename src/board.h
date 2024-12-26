#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "piece.h"

class Board {
public:
    Board();
    void draw();
    void initializePieces();
    std::vector<Piece>& getPieces();
private:
    std::vector<Piece> pieces;
};

#endif
