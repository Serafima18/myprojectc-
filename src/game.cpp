#include "game.h"

Game::Game(int x, int y, int w, int h, const char* title) : Fl_Window(x, y, w, h, title) {
    board = new Board(10, 10, w - 20, h - 20);
    board->box(FL_DOWN_FRAME);
    board->color(FL_GRAY);
    end();
}