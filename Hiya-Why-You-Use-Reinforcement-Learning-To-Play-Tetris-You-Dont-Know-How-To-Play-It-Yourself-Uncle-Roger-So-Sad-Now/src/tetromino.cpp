#include "tetromino.h"
#include "log.h"
#include "tetriscontroller.h"

#include <cstring>
#include <algorithm>
#include <allegro5/allegro_primitives.h>

Tetromino::Tetromino(Tile t, Board &b) : type(t), board(b) {
    memcpy(block, block_types[int(type)], sizeof(block));
    block_size = block_sizes[int(type)];

    if (type == Tile::SKY)
        is_I_block = true;

    x = (TILE_COUNT_H - block_size) / 2;

    for (int i = TILE_COUNT_V - block_size; i < TILE_COUNT_V + block_size; i++) {
        if (CheckFree(x, i)) {
            y = i;
            creation_success = true;
            break;
        }
    }
}

void Tetromino::Draw() {
    int to_ground = DistanceToGround();
    DrawBlocks(x, y - to_ground, TETROMINO_GHOST_ALPHA); // Ghost

    DrawBlocks(x, y, 1.0);
}

void Tetromino::DrawBlocks(int posx, int posy, double alpha) {
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            if (block[i][j] == Tile::NONE)
                continue;


            const int px = posx + j, py = posy + (block_size - i - 1);
//            al_draw_filled_rectangle(GAMEPLAY_X + TILE_SIZE * px, GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - py - 1),
//                                     GAMEPLAY_X + TILE_SIZE * (px + 1), GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - py),
//                                     al_map_rgb(244, 128, 36));
            al_draw_tinted_scaled_bitmap(TetrisController::tetrimino_textures[int(block[i][j])],
                                  al_map_rgba_f(alpha, alpha, alpha, alpha),
                                  0, 0, TETROMINO_BLOCK_TEXTURE_SIZE, TETROMINO_BLOCK_TEXTURE_SIZE,
                                  GAMEPLAY_X + TILE_SIZE * px, GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - py - 1),
                                  TILE_SIZE, TILE_SIZE,
                                  0);
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
            if (board[py][px] != Tile::NONE)
                return false;
        }
    }
    return true;
}

void Tetromino::Fall() {
    if (!CanFall())
        return;

    y--;
}

void Tetromino::Place() {
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            if (block[i][j] == Tile::NONE)
                continue;

            const int px = x + j, py = y + (block_size - i - 1);
            board[py][px] = block[i][j];
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
                    if (board[k][px] != Tile::NONE)
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
        auto [dx, dy] = is_I_block? I_wall_kick_offsets[kick_id][i]: wall_kick_offsets[kick_id][i];
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

bool Tetromino::CanFall() {
    return CheckFree(x, y - 1);
}

void Tetromino::HardFall() {
    y -= DistanceToGround();
}

bool Tetromino::Success() {
    return creation_success;
}
