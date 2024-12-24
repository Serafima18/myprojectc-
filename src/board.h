#ifndef CHECKERS_BOARD_H
#define CHECKERS_BOARD_H

#include "C:/Users/89853/Desktop/Git/myprojectc++/FLTK/include/FL/Fl_Widget.H"
#include <vector>
#include "piece.h"

class Board : public Fl_Widget {
public:
    Board(int x, int y, int w, int h);
    void draw() override;
    int handle(int event) override;

private:
    void drawPiece(int row, int col, const Piece& piece) const;
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool canCapture(int fromRow, int fromCol, int toRow, int toCol) const;
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    void selectPiece(int row, int col);
    void unselectPiece();

    int size;
    int selectedRow, selectedCol;
    bool pieceSelected;
    std::vector<std::vector<Piece>> board;
};

#endif // CHECKERS_BOARD_H