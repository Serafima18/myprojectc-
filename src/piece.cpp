#include "piece.h"
#include "./../fltk/include/FL/fl_draw.H"

Piece::Piece(Color color, int x, int y) : color(color), x(x), y(y) {}

void Piece::draw() {
    fl_color(color == Color::RED ? FL_RED : FL_WHITE);
    fl_pie(x * 100 + 10, y * 100 + 10, 80, 80, 0, 360);
}

Color Piece::getColor() const {
    return color;
}

void Piece::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

int Piece::getX() const {
    return x;
}

int Piece::getY() const {
    return y;
}
