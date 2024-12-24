#include "board.h"
#include "C:/Users/89853/Desktop/Git/myprojectc++/FLTK/include/FL/Fl.H"
#include "C:/Users/89853/Desktop/Git/myprojectc++/FLTK/include/FL/fl_draw.H"

Board::Board(int x, int y, int w, int h) : Fl_Widget(x, y, w, h), size(8), selectedRow(-1), selectedCol(-1), pieceSelected(false) {
    board.resize(size, std::vector<Piece>(size, Piece(PieceType::EMPTY)));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if ((i + j) % 2 == 0) {
                board[i][j] = Piece(PieceType::EMPTY);
            } else {
                board[i][j] = (i < 3) ? Piece(PieceType::RED) : (i >= 5) ? Piece(PieceType::WHITE) : Piece(PieceType::EMPTY);
            }
        }
    }
}

void Board::draw() {
    int cellSize = w() / size;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if ((i + j) % 2 == 0) {
                fl_color(FL_LIGHT2);
            } else {
                fl_color(FL_DARK3);
            }
            fl_rectf(j * cellSize, i * cellSize, cellSize, cellSize);
            drawPiece(i, j, board[i][j]);
        }
    }
}

void Board::drawPiece(int row, int col, const Piece& piece) const {
    int cellSize = w() / size;
    int x = col * cellSize;
    int y = row * cellSize;

    switch (piece.getType()) {
        case PieceType::RED:
            fl_color(FL_RED);
            break;
        case PieceType::WHITE:
            fl_color(FL_WHITE);
            break;
        default:
            return;
    }

    fl_pie(x, y, cellSize, cellSize, 0, 360);

    if (piece.isKing()) {
        fl_color(FL_YELLOW);
        fl_pie(x + cellSize / 4, y + cellSize / 4, cellSize / 2, cellSize / 2, 0, 360);
    }
}

int Board::handle(int event) {
    if (event == FL_PUSH) {
        int x = Fl::event_x();
        int y = Fl::event_y();

        int cellSize = w() / size;

        int row = y / cellSize;
        int col = x / cellSize;

        if (pieceSelected) {
            if (isValidMove(selectedRow, selectedCol, row, col)) {
                movePiece(selectedRow, selectedCol, row, col);
                unselectPiece();
            }
        } else {
            selectPiece(row, col);
        }
        redraw();
    }
    return Fl_Widget::handle(event);
}

bool Board::isValidMove(int fromRow, int fromCol, int toRow, int toCol) const {
    if (fromRow == toRow || fromCol == toCol) return false;
    if (board[toRow][toCol].getType() != PieceType::EMPTY) return false;

    Piece piece = board[fromRow][fromCol];
    int dRow = toRow - fromRow;
    int dCol = toCol - fromCol;

    if (piece.isKing()) {
        return (std::abs(dRow) == std::abs(dCol)) && (std::abs(dRow) == 1 || (std::abs(dRow) == 2 && canCapture(fromRow, fromCol, toRow, toCol)));
    } else {
        if (piece.getType() == PieceType::RED && dRow > 0) return false;
        if (piece.getType() == PieceType::WHITE && dRow < 0) return false;
        return (std::abs(dRow) == 1 && std::abs(dCol) == 1) || (std::abs(dRow) == 2 && std::abs(dCol) == 2 && canCapture(fromRow, fromCol, toRow, toCol));
    }
}

bool Board::canCapture(int fromRow, int fromCol, int toRow, int toCol) const {
    int dRow = (toRow - fromRow) / 2;
    int dCol = (toCol - fromCol) / 2;
    int midRow = fromRow + dRow;
    int midCol = fromCol + dCol;

    return board[midRow][midCol].getType() != PieceType::EMPTY && board[midRow][midCol].getType() != board[fromRow][fromCol].getType();
}

void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    Piece piece = board[fromRow][fromCol];
    board[fromRow][fromCol] = Piece(PieceType::EMPTY);

    if (std::abs(toRow - fromRow) == 2) {
        int midRow = (toRow + fromRow) / 2;
        int midCol = (toCol + fromCol) / 2;
        board[midRow][midCol] = Piece(PieceType::EMPTY);
    }

    board[toRow][toCol] = piece;

    if (toRow == 0 && piece.getType() == PieceType::WHITE) {
        board[toRow][toCol].setKing(true);
    } else if (toRow == size - 1 && piece.getType() == PieceType::RED) {
        board[toRow][toCol].setKing(true);
    }
}

void Board::selectPiece(int row, int col) {
    if (board[row][col].getType() != PieceType::EMPTY) {
        selectedRow = row;
        selectedCol = col;
        pieceSelected = true;
    }
}

void Board::unselectPiece() {
    pieceSelected = false;
}