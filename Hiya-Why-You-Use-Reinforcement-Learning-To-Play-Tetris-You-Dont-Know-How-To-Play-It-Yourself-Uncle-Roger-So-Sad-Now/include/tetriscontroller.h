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
    void Fall();
    void HardFall();

    void Place();

    void NextTetromino();

    void CheckLines();
    void ClearLine(int y);

    static ALLEGRO_BITMAP *hold_text;
    static ALLEGRO_BITMAP *next_text;

    static ALLEGRO_BITMAP *tetrimino_textures[9];


 private:
    Board board;

    TetrisState state;

    Tetromino *falling = nullptr;
    std::deque<Tile> next_queue;
    Tile hold = Tile::NONE;
    bool last_hold = false;

    static bool textures_loaded;


    int remaining_regret_times;
};