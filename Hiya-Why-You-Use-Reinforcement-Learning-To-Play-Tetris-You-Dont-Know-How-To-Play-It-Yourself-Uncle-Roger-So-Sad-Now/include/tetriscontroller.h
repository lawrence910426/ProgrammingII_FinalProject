#include "globals.h"

using namespace Constants;

#include "tetromino.h"


class TetrisController {
 public:
    TetrisController();

    TetrisState Next();

    void Draw();

    bool Rotate(bool ccw);
    bool Hold();
    bool Move(bool left);
    bool Fall();
    bool HardFall();

    void Place();

    void NextTetromino();

 private:
    Board board;

    TetrisState state;

    Tetromino *falling = nullptr;
    Tetromino *next[5];
    Tetromino *hold;



    int remaining_regret_times;
};