#ifndef CHECKERS_PIECE_H
#define CHECKERS_PIECE_H

enum class PieceType {
    EMPTY,
    RED,
    WHITE
};

class Piece {
    PieceType type;
    bool king;
public:
    Piece(PieceType type, bool king = false) : type(type), king(king) {}
    PieceType getType() const { return type; }
    bool isKing() const { return king; }
    void setKing(bool king) { king = king; }
};

#endif // CHECKERS_PIECE_H