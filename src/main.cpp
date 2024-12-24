#include "C:/Users/89853/Desktop/Git/myprojectc++/FLTK/include/FL/Fl.H"
#include "game.h"

int main(int argc, char** argv) {
    Game game(100, 100, 600, 600, "Checkers");
    game.show();
    return Fl::run();
}