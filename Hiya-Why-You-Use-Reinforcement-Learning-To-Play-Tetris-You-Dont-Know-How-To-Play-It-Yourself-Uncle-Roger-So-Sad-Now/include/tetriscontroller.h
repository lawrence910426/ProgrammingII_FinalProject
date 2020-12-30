#include "globals.h"

using namespace Constants;

#include "tetromino.h"


class TetrisController {
 public:
    TetrisController();
    ~TetrisController();

    TetrisState Next();

    void Draw();

    // Returns true if success
    bool Rotate(bool ccw);
    bool Hold();
    bool Move(bool left);
    bool Fall();
    bool HardFall();

 private:
    Board rows;

    TetrisState state;

    Tetromino *falling;
    Tetromino *next[5];
    Tetromino *hold;
};