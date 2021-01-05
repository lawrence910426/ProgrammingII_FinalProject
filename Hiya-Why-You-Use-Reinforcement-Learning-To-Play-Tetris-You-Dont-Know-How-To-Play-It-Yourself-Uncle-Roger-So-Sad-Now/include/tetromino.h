#pragma once
#include "globals.h"

using namespace Constants;

#include <utility>

class Tetromino {
 public:
    Tetromino(Tile type, Board &board);

    void Draw();
    void DrawBlocks(int posx, int posy, double alpha);

    bool Rotate(bool ccw);

    void RotateBlock(bool ccw);
    void Place();

    bool CheckFree(int dx, int dy);
    bool CanFall();

    bool Move(bool left);

    int DistanceToGround();

    void Fall();
    void HardFall();

    bool Success() const;


    int x = -1, y = -1;

    int orientation = 0;
    bool is_I_block = false;

    int block_size;
    Tile block[4][4];

    Tile type;

    bool creation_success = false;


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

    constexpr static const std::pair<int, int> wall_kick_offsets[][5] ={
        { {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} },
        { {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2} },
        { {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2} },
        { {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} }
    };

    constexpr static const std::pair<int, int> I_wall_kick_offsets[][5] ={
        { {0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2} },
        { {0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1} },
        { {0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2} },
        { {0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1} }
    };


 private:
    Board &board;
};

