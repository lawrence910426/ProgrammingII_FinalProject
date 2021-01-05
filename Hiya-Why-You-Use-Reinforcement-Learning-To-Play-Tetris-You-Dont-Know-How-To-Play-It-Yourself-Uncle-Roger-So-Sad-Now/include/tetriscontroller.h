#pragma once
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

    void CheckLines();
    void ClearLine(int y);

    static ALLEGRO_BITMAP *tetrimino_textures[9];

 private:
    Board board;

    TetrisState state;

    Tetromino *falling = nullptr;
    Tetromino *next[5];
    Tetromino *hold;

    static bool textures_loaded;


    int remaining_regret_times;
};