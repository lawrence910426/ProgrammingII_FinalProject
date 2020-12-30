#include "globals.h"

using namespace Constants;

class Tetromino {
 public:
    Tetromino();
    ~Tetromino();

    void DrawActive();
    void DrawPreview();
    void DrawHold();

    void Rotate(Board&, bool ccw);
    void Place(Board&);

    int x, y;

    int orientation;

    int block_size;
    Tile block[4][4];
};