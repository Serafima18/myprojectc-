#include "board.h"
#include "./../fltk/include/FL/fl_draw.H"

Board::Board() {
    initializePieces();
}

void Board::initializePieces() {
    for (int i = 0; i < 3; ++i) {
        for (int j = (i % 2); j < 8; j += 2) {
            pieces.emplace_back(Color::RED, j, i);
        }
    }
    for (int i = 5; i < 8; ++i) {
        for (int j = (i % 2); j < 8; j += 2) {
            pieces.emplace_back(Color::WHITE, j, i);
        }
    }
}

std::vector<Piece>& Board::getPieces() {
    return pieces;
}

void Board::draw() {
    // Draw checkerboard
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            fl_color((i + j) % 2 == 0 ? FL_WHITE : FL_GRAY);
            fl_rectf(j * 100, i * 100, 100, 100);
        }
    }

    // Draw pieces
    for (auto& piece : pieces) {
        piece.draw();
    }
}
