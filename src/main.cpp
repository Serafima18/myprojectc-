#include "./../fltk/include/FL/Fl.H"
#include "./../fltk/include/FL/Fl_Window.H"
#include "./../fltk/include/FL/fl_draw.H"
#include <vector>
#include <algorithm>
#include <iostream>

const int BOARD_SIZE = 8;
const int CELL_SIZE = 60;

enum CheckerColor { RED, BLACK };

struct Checker {
    int x, y;
    bool isKing;
    CheckerColor color;
};

class CheckersGame : public Fl_Widget {
public:
    CheckersGame(int X, int Y, int W, int H) 
        : Fl_Widget(X, Y, W, H), currentPlayer(RED) {
        // Установим начальное расположение шашек
        for (int i = 0; i < BOARD_SIZE; i += 2) {
            for (int j = 0; j < 3; j++) {
                if (j % 2 == 0) {
                    checkers.push_back({i + 1, j, false, BLACK});
                    checkers.push_back({i, BOARD_SIZE - j - 1, false, RED});
                } else {
                    checkers.push_back({i, j, false, BLACK});
                    checkers.push_back({i + 1, BOARD_SIZE - j - 1, false, RED});
                }
            }
        }
        selectedChecker = nullptr;
    }

    // Подсветка должна быть для текущего игрока
    void draw() override {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if ((i + j) % 2 == 0)
                    fl_color(FL_WHITE);
                else
                    fl_color(FL_DARK2);

                fl_rectf(x() + i * CELL_SIZE, y() + j * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            }
        }

        // Подсветить допустимые ходы, если выбранная шашка соответствует игроку
        if (selectedChecker && selectedChecker->color == currentPlayer) {
            auto possibleMoves = getPossibleMoves(selectedChecker);
            fl_color(FL_YELLOW);
            for (auto move : possibleMoves) {
                fl_rectf(x() + move.first * CELL_SIZE, y() + move.second * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            }
        }

        for (const auto& checker : checkers) {
            if (checker.color == RED)
                fl_color(FL_RED);
            else
                fl_color(FL_BLACK);

            int offset = (CELL_SIZE - (CELL_SIZE - 10)) / 2;
            fl_pie(x() + checker.x * CELL_SIZE + offset, y() + checker.y * CELL_SIZE + offset, CELL_SIZE - 10, CELL_SIZE - 10, 0, 360);
            
            // Обозначение дамки
            if (checker.isKing) {
                fl_color(FL_BLUE);
                fl_pie(x() + checker.x * CELL_SIZE + offset + 10, y() + checker.y * CELL_SIZE + offset + 10, CELL_SIZE - 30, CELL_SIZE - 30, 0, 360);
            }
        }
    }

    int handle(int event) override {
        switch (event) {
            case FL_PUSH:
                return onMouseClick();
            default:
                return 0;
        }
    }

private:
    std::vector<Checker> checkers;
    Checker* selectedChecker;
    CheckerColor currentPlayer;

    int onMouseClick() {
        int mx = Fl::event_x();
        int my = Fl::event_y();
        
        int grid_x = (mx - x()) / CELL_SIZE;
        int grid_y = (my - y()) / CELL_SIZE;

        Checker* clickedChecker = getCheckerAt(grid_x, grid_y);

        if (selectedChecker) {
            if (isMoveValid(selectedChecker, grid_x, grid_y)) {
                moveChecker(selectedChecker, grid_x, grid_y);
            } else if (clickedChecker && clickedChecker->color == currentPlayer) {
                // Смена выбранной шашки, если это шашка текущего игрока
                selectedChecker = clickedChecker;
            } else {
                // Сброс выделения, если клик невалиден
                selectedChecker = nullptr;
            }
        } else {
            if (clickedChecker && clickedChecker->color == currentPlayer) {
                selectedChecker = clickedChecker;
            }
        }

        redraw();
        return 1;
    }

    Checker* getCheckerAt(int grid_x, int grid_y) {
        for (auto& checker : checkers) {
            if (checker.x == grid_x && checker.y == grid_y) {
                return &checker;
            }
        }
        return nullptr;
    }

    bool isMoveValid(Checker* checker, int new_x, int new_y) {
        if (!checker || getCheckerAt(new_x, new_y)) {
            return false; // Нельзя перемещаться на занятую клетку
        }

        int dx = new_x - checker->x;
        int dy = new_y - checker->y;

        // Проверки для обычных пешек
        if (!checker->isKing) {
            if (std::abs(dx) == 2 && std::abs(dy) == 2) {
                int jump_x = checker->x + dx / 2;
                int jump_y = checker->y + dy / 2;
                Checker* enemy = getCheckerAt(jump_x, jump_y);
                if (enemy && enemy->color != checker->color) {
                    // Есть противник, через который можно перепрыгнуть
                    removeCheckerAt(jump_x, jump_y);
                    return true;
                }
            }
            if (std::abs(dx) == 1) {
                if ((checker->color == RED && dy == -1) || (checker->color == BLACK && dy == 1)) {
                    return true; 
                }
            }
        } else {
            // Проверки для дамок
            if (std::abs(dx) == std::abs(dy)) {
                int step_x = dx / std::abs(dx);
                int step_y = dy / std::abs(dy);

                // Проверка для возможного захвата
                int checker_steps = std::abs(dx);
                for (int i = 1; i < checker_steps; ++i) {
                    int intermediate_x = checker->x + i * step_x;
                    int intermediate_y = checker->y + i * step_y;
                    Checker* intermediate_checker = getCheckerAt(intermediate_x, intermediate_y);
                    if (intermediate_checker) {
                        // Если нашли промежуточную шашку противника
                        if (intermediate_checker->color != checker->color) {
                            // Проверяем, возможно ли движение за нее
                            int target_x = intermediate_x + step_x;
                            int target_y = intermediate_y + step_y;

                            // Проверяем, не выходит ли за границы
                            if (target_x >= 0 && target_x < BOARD_SIZE && 
                                target_y >= 0 && target_y < BOARD_SIZE && 
                                !getCheckerAt(target_x, target_y)) {
                                return true; // Дамка может захватить
                            }
                            break; // Дальше без толка
                        }
                        break; // Обычная шашка встречает препятствие, нет смысла продолжать
                    }
                }
            }
        }
        return false;
    }

    void moveChecker(Checker* checker, int new_x, int new_y) {
        int dx = new_x - checker->x;
        int dy = new_y - checker->y;
        bool captured = false;

        // Если прыжок (то есть съем шашки)
        if (std::abs(dx) == 2 && std::abs(dy) == 2) {
            int jump_x = checker->x + dx / 2;
            int jump_y = checker->y + dy / 2;
            removeCheckerAt(jump_x, jump_y);
            captured = true;
        }

        // Перемещаем шашку
        checker->x = new_x;
        checker->y = new_y;

        // Превращаем в дамку, если шашка достигла противоположного конца доски
        if (!checker->isKing && ((checker->color == RED && new_y == 0) || (checker->color == BLACK && new_y == BOARD_SIZE - 1))) {
            checker->isKing = true;
        }

        if (captured) {
            if (canContinueJumping(checker)) {
                // Остаемся на этом шаге, потому что мы можем продолжать прыжок
                selectedChecker = checker;
                return; // Прекращаем выполнение, чтобы игрок мог продолжать ход
            }
        }

        // Сброс выбранной шашки и смена игрока, если больше нет захвата
        selectedChecker = nullptr;
        currentPlayer = (currentPlayer == RED) ? BLACK : RED;
    }

    void removeCheckerAt(int grid_x, int grid_y) {
        checkers.erase(std::remove_if(checkers.begin(), checkers.end(), 
            [grid_x, grid_y](const Checker& c){ return c.x == grid_x && c.y == grid_y; }), checkers.end());
    }

    std::vector<std::pair<int, int>> getPossibleMoves(Checker* checker) {
        std::vector<std::pair<int, int>> moves;
        if (!checker) return moves;

        if (!checker->isKing) {
            int direction = (checker->color == RED) ? -1 : 1;
            for (int dx = -1; dx <= 1; dx += 2) {
                int nx = checker->x + dx;
                int ny = checker->y + direction;
                if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && !getCheckerAt(nx, ny)) {
                    moves.emplace_back(nx, ny);
                }
                nx = checker->x + 2 * dx;
                ny = checker->y + 2 * direction;
                if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && !getCheckerAt(nx, ny)) {
                    int jump_x = checker->x + dx;
                    int jump_y = checker->y + direction;
                    Checker* enemy = getCheckerAt(jump_x, jump_y);
                    if (enemy && enemy->color != checker->color) {
                        moves.emplace_back(nx, ny);
                    }
                }
            }
        } else {
            int directions[] = {-1, 1};
            for (int dx : directions) {
                for (int dy : directions) {
                    for (int i = 1; i < BOARD_SIZE; ++i) {
                        int nx = checker->x + i * dx;
                        int ny = checker->y + i * dy;
                        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                            if (!getCheckerAt(nx, ny)) {
                                moves.emplace_back(nx, ny);
                            } else {
                                if (getCheckerAt(nx, ny)->color != checker->color) {
                                    moves.emplace_back(nx + dx, ny + dy);
                                }
                                break; // Не можем пойти дальше после шашки
                            }
                        } else {
                            break; // Вышли за пределы доски
                        }
                    }
                }
            }
        }
        return moves;
    }
    bool canContinueJumping(Checker* checker) {
        int direction[2] = {-2, 2};  // Проверка в обе стороны для дамок и простых

        for (int dx : direction) {
            for (int dy : direction) {
                int nx = checker->x + dx;
                int ny = checker->y + dy;
                int jump_x = checker->x + dx / 2;
                int jump_y = checker->y + dy / 2;
                Checker* enemy = getCheckerAt(jump_x, jump_y);
                if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && !getCheckerAt(nx, ny) && enemy && enemy->color != checker->color) {
                    return true;
                }
            }
        }
        return false;
    }
};

int main() {
    Fl_Window window(BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE, "Checkers Game");
    CheckersGame game(0, 0, BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE);
    window.end();
    window.show();
    return Fl::run();
}
