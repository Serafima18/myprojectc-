#ifndef CELL_H
#define CELL_H

#include "./../fltk/include/FL/Fl.H"
#include "./../fltk/include/FL/Fl_Window.H"
#include "./../fltk/include/FL/Fl_Box.H"
#include "board.h"

class Cell : public Fl_Box {
public:
    Cell(int X, int Y, int W, int H, const Board& cell);
    void draw() override;

private:
    Board cell;
};

#endif
