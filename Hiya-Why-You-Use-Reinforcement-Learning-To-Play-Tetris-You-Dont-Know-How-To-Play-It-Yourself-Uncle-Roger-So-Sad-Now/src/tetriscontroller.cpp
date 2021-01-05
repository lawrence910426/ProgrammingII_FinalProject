#include "tetriscontroller.h"

#include <allegro5/allegro_primitives.h>
#include <random>
#include "log.h"

bool TetrisController::textures_loaded = false;
ALLEGRO_BITMAP *TetrisController::tetrimino_textures[9];

TetrisController::TetrisController() {
    for (int i = 0; i < TILE_COUNT_V + 5; i++)
        board.emplace_back(std::vector<Tile>(TILE_COUNT_H, Tile::NONE));

    state = TetrisState::LANDED;


    if (!textures_loaded) {
        tetrimino_textures[int(Tile::RED)] = al_load_bitmap("../assets/block-red.png");
        tetrimino_textures[int(Tile::ORANGE)] = al_load_bitmap("../assets/block-orange.png");
        tetrimino_textures[int(Tile::YELLOW)] = al_load_bitmap("../assets/block-yellow.png");
        tetrimino_textures[int(Tile::GREEN)] = al_load_bitmap("../assets/block-green.png");
        tetrimino_textures[int(Tile::BLUE)] = al_load_bitmap("../assets/block-blue.png");
        tetrimino_textures[int(Tile::SKY)] = al_load_bitmap("../assets/block-sky.png");
        tetrimino_textures[int(Tile::PURPLE)] = al_load_bitmap("../assets/block-purple.png");
        tetrimino_textures[int(Tile::GRAY)] = al_load_bitmap("../assets/block-gray.png");
        textures_loaded = true;
    }
}

void TetrisController::Draw() {
    if (falling != nullptr)
        falling->DrawActive();
    for (int i = 0; i < TILE_COUNT_V; i++) {
        for (int j = 0; j < TILE_COUNT_H; j++) {
            if (board[i][j] == Tile::NONE)
                continue;
//            al_draw_filled_rectangle(GAMEPLAY_X + TILE_SIZE * j, GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - i - 1),
//                                     GAMEPLAY_X + TILE_SIZE * (j + 1), GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - i),
//                                     al_map_rgb(244, 128, 36));
            al_draw_scaled_bitmap(tetrimino_textures[int(board[i][j])],
                                  0, 0, TETROMINO_BLOCK_TEXTURE_SIZE, TETROMINO_BLOCK_TEXTURE_SIZE,
                                  GAMEPLAY_X + TILE_SIZE * j, GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - i - 1),
                                  TILE_SIZE, TILE_SIZE,
                                  0);
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
        const bool landing = Fall();
        if (landing && remaining_regret_times <= 0)
            Place();
    } else if (state == TetrisState::LANDING) {
        if (falling->CanFall()) {
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
    CheckLines();
}

void TetrisController::CheckLines() {
    for (int i = 0; i < board.size(); i++) {
        if (std::any_of(board[i].begin(), board[i].end(),
                        [](Tile t){return t == Tile::NONE;}))
            continue;
        ClearLine(i);
        i--;
    }
}

void TetrisController::ClearLine(int y) {
    board.erase(board.begin() + y);
    board.emplace_back(std::vector<Tile>(TILE_COUNT_H, Tile::NONE));
}
