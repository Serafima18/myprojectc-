#ifndef CHECKERS_GAME_H
#define CHECKERS_GAME_H

#include "C:\Users\89853\Desktop\Git\myprojectc++\FLTK\include\FL\Fl_Window.H"
#include "board.h"

class Game : public Fl_Window {
public:
    Game(int x, int y, int w, int h, const char* title);

private:
    Board* board;
};

#endif // CHECKERS_GAME_H