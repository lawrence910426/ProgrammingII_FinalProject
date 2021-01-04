#include "tetromino.h"
#include "log.h"

#include <cstring>
#include <algorithm>
#include <allegro5/allegro_primitives.h>


Tetromino::Tetromino(int type, Board *b) {
    memcpy(block, block_types[type], sizeof(block));
    block_size = block_sizes[type];

    if (type == 6)
        is_I_block = true;

    board = b;
}

void Tetromino::DrawActive() {
    DrawGhost();
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            if (block[i][j] == Tile::NONE)
                continue;


            const int px = x + j, py = y + (block_size - i - 1);
            al_draw_filled_rectangle(GAMEPLAY_X + TILE_SIZE * px, GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - py - 1),
                                     GAMEPLAY_X + TILE_SIZE * (px + 1), GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - py),
                                     al_map_rgb(244, 128, 36));
        }
    }
}


bool Tetromino::Move(bool left) {
    const int nx = left? x - 1: x + 1;
    if (!CheckFree(nx, y))
        return false;

    x = nx;
    return true;
}

bool Tetromino::CheckFree(int nx, int ny) {
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            if (block[i][j] == Tile::NONE)
                continue;

            const int px = nx + j, py = ny + (block_size - i - 1);
            if (px < 0 || px >= TILE_COUNT_H || py < 0)
                return false;
            if ((*board)[py][px] != Tile::NONE)
                return false;
        }
    }
    return true;
}

bool Tetromino::Fall() {
//    INFO(x << " " << y << " " << CheckFree(x, y - 1));
    if (!CheckFree(x, y - 1))
        return true;

    y--;
    return false;
}

void Tetromino::Place() {
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            if (block[i][j] == Tile::NONE)
                continue;

            const int px = x + j, py = y + (block_size - i - 1);
            (*board)[py][px] = block[i][j];
        }
    }
}

void Tetromino::DrawGhost() {
    int to_ground = DistanceToGround();
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            if (block[i][j] == Tile::NONE)
                continue;


            const int px = x + j, py = y + (block_size - i - 1) - to_ground;
            al_draw_rectangle(GAMEPLAY_X + TILE_SIZE * px, GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - py - 1),
                                     GAMEPLAY_X + TILE_SIZE * (px + 1), GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - py),
                                     al_map_rgb(244, 128, 36), GIRD_WIDTH);
        }
    }
}

int Tetromino::DistanceToGround() {
    int to_ground = TILE_COUNT_V * 2;

    for (int j = 0; j < block_size; j++) {
        for (int i = 0; i < block_size; i++) {
            if (block[i][j] != Tile::NONE) {
                const int px = x + j, py = y + (block_size - i - 1);
                int d = 0;
                for (int k = py - 1; k >= 0; k--) {
                    if ((*board)[k][px] != Tile::NONE)
                        break;
                    d++;
                }
                to_ground = std::min(to_ground, d);
            }
        }
    }
    return to_ground;
}



bool Tetromino::Rotate(bool ccw) {
    RotateBlock(ccw);

    int kick_id;
    if (ccw) {
        orientation = (orientation - 1 + orientation) % 4;
        kick_id = orientation;
    } else {
        kick_id = orientation;
        orientation = (orientation + 1) % 4;
    }

    for (int i = 0; i < 5; i++) {
        int dx, dy;
        std::tie(dx, dy) = is_I_block? I_wall_kick_offsets[kick_id][i]: wall_kick_offsets[kick_id][i];
        if (ccw) {
            dx = -dx;
            dy = -dy;
        }

        if (CheckFree(x + dx, y + dy)) {
            x += dx;
            y += dy;
            return true;
        }
    }

    RotateBlock(!ccw);
    if (ccw)
        orientation = (orientation + 1) % 4;
    else
        orientation = (orientation - 1 + orientation) % 4;
    return false;
}

void Tetromino::RotateBlock(bool ccw) {
    Tile new_block[4][4] = {};
    if (ccw) {
        for (int i = 0; i < block_size; i++)
            for (int j = 0; j < block_size; j++)
                new_block[i][j] = block[j][block_size - i - 1];
    } else {
        for (int i = 0; i < block_size; i++)
            for (int j = 0; j < block_size; j++)
                new_block[i][j] = block[block_size - j - 1][i];
    }
    memcpy(block, new_block, sizeof(block));
}
