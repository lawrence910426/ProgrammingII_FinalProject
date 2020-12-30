#include "globals.h"

using namespace Constants;



class Tetromino {
 public:
    Tetromino(int type);

    void DrawActive();
    void DrawPreview();
    void DrawHold();

    void Rotate(Board&, bool ccw);
    void Place(Board&);

    int x = -1, y = -1;

    int orientation = 0;
    bool is_I_block = false;

    int block_size;
    Tile block[4][4];


    constexpr static const Tile block_types[][4][4] = {
        {}, // None
        {   // Z Block
            {Tile::RED,  Tile::RED,  Tile::NONE},
            {Tile::NONE, Tile::RED,  Tile::RED},
            {Tile::NONE, Tile::NONE, Tile::NONE}
        },
        {   // L Block
            {Tile::NONE,   Tile::NONE,   Tile::ORANGE},
            {Tile::ORANGE, Tile::ORANGE, Tile::ORANGE},
            {Tile::NONE,   Tile::NONE,   Tile::NONE}
        },
        {   // O Block
            {Tile::YELLOW, Tile::YELLOW},
            {Tile::YELLOW, Tile::YELLOW}
        },
        {   // S Block
            {Tile::NONE,  Tile::GREEN, Tile::GREEN},
            {Tile::GREEN, Tile::GREEN, Tile::NONE},
            {Tile::NONE,  Tile::NONE,  Tile::NONE}
        },
        {   // L Block
            {Tile::BLUE, Tile::NONE, Tile::NONE},
            {Tile::BLUE, Tile::BLUE, Tile::BLUE},
            {Tile::NONE, Tile::NONE, Tile::NONE}
        },
        {   // I Block
            {Tile::NONE, Tile::NONE, Tile::NONE, Tile::NONE},
            {Tile::SKY,  Tile::SKY,  Tile::SKY,  Tile::SKY},
            {Tile::NONE, Tile::NONE, Tile::NONE, Tile::NONE},
            {Tile::NONE, Tile::NONE, Tile::NONE, Tile::NONE}
        },
        {   // T Block
            {Tile::NONE,   Tile::PURPLE, Tile::NONE},
            {Tile::PURPLE, Tile::PURPLE, Tile::PURPLE},
            {Tile::NONE,   Tile::NONE,   Tile::NONE}
        }
    };

    constexpr static const int block_sizes[] = {
        0, 3, 3, 2, 3, 3, 4, 3
    };
};

