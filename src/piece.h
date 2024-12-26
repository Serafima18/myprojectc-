#ifndef PIECE_H
#define PIECE_H

enum class Color { RED, WHITE };

class Piece {
public:
    Piece(Color color, int x, int y);
    void draw();

    Color getColor() const;
    void setPosition(int x, int y);
    int getX() const;
    int getY() const;

private:
    Color color;
    int x, y;
};

#endif
