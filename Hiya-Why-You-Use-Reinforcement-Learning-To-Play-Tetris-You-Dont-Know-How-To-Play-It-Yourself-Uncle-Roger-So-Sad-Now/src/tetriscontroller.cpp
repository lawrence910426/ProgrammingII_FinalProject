#include "tetriscontroller.h"

#include <allegro5/allegro_primitives.h>
#include <random>
#include "log.h"


TetrisController::TetrisController() {
    for (int i = 0; i < TILE_COUNT_V + 5; i++)
        board.emplace_back(std::vector<Tile>(TILE_COUNT_H, Tile::NONE));

    state = TetrisState::LANDED;
}

void TetrisController::Draw() {
    if (falling != nullptr)
        falling->DrawActive();
    for (int i = 0; i < TILE_COUNT_V; i++) {
        for (int j = 0; j < TILE_COUNT_H; j++) {
            if (board[i][j] == Tile::NONE)
                continue;
            al_draw_filled_rectangle(GAMEPLAY_X + TILE_SIZE * j, GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - i - 1),
                                     GAMEPLAY_X + TILE_SIZE * (j + 1), GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - i),
                                     al_map_rgb(244, 128, 36));
        }
    }
}

bool TetrisController::Hold() {
    return false;
}

bool TetrisController::Rotate(bool ccw) {
    if (falling == nullptr)
        return false;
    return falling->Rotate(ccw);
}

bool TetrisController::Move(bool left) {
    if (falling == nullptr)
        return false;
    if (remaining_regret_times > 0 && state == TetrisState::LANDING) {
        remaining_regret_times--;
        state = TetrisState::FALLING;
    }
    return falling->Move(left);
}

bool TetrisController::Fall() {
    if (falling == nullptr)
        return false;
    bool landing = falling->Fall();
    if (landing)
        state = TetrisState::LANDING;
    return falling;
}

bool TetrisController::HardFall() {
    if (state == TetrisState::FALLING) {
        falling->y -= falling->DistanceToGround();
        Place();
    }
    return true;
}

TetrisState TetrisController::Next() {
//    if (falling != nullptr)
//        INFO(falling->x << " " << falling->y);
    if (state == TetrisState::LANDED) {
        NextTetromino();
        state = TetrisState::FALLING;
    } else if (state == TetrisState::FALLING) {
        Fall();
    } else if (state == TetrisState::LANDING) {
        if (remaining_regret_times > 0 && falling->CheckFree(falling->x, falling->y - 1)) {
            state = TetrisState::FALLING;
            remaining_regret_times--;
        } else {
            Place();
        }
    }
    return state;
}

void TetrisController::NextTetromino() {
    falling = new Tetromino(randint(1, 7), &board);
    falling->y = 16;
    do {
        falling->x = randint(0, TILE_COUNT_H - 1);
    } while (!falling->CheckFree(falling->x, falling->y));
    remaining_regret_times = LANDING_REGRET_TIMES;
}

void TetrisController::Place() {
    falling->Place();
    delete falling;
    falling = nullptr;
    state = TetrisState::LANDED;
}
