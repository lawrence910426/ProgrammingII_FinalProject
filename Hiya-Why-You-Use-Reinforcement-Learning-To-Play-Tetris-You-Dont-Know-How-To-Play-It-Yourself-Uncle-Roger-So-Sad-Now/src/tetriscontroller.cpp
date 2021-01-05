#include "tetriscontroller.h"

#include <allegro5/allegro_primitives.h>
#include <random>
#include "log.h"

bool TetrisController::textures_loaded = false;
ALLEGRO_BITMAP *TetrisController::tetrimino_textures[9];

TetrisController::TetrisController() {
    for (int i = 0; i < TILE_COUNT_V + 5; i++)
        board.emplace_back(std::vector<Tile>(TILE_COUNT_H, Tile::NONE));

    for (int i = 0; i < PREVIEW_COUNT; i++)
        next_queue.emplace(Tile(randint(1, 7)));

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
        falling->Draw();
    for (int i = 0; i < TILE_COUNT_V; i++) {
        for (int j = 0; j < TILE_COUNT_H; j++) {
            if (board[i][j] == Tile::NONE)
                continue;
//            al_draw_filled_rectangle(GAMEPLAY_X + TILE_SIZE * j, GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - i - 1),
//                                     GAMEPLAY_X + TILE_SIZE * (j + 1), GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - i),
//                                     al_map_rgb(244, 128, 36));
            al_draw_tinted_scaled_bitmap(tetrimino_textures[int(board[i][j])],
                                         al_map_rgba_f(TETROMINO_BOARD_ALPHA, TETROMINO_BOARD_ALPHA, TETROMINO_BOARD_ALPHA, TETROMINO_BOARD_ALPHA),
                                         0, 0, TETROMINO_BLOCK_TEXTURE_SIZE, TETROMINO_BLOCK_TEXTURE_SIZE,
                                         GAMEPLAY_X + TILE_SIZE * j, GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - i - 1),
                                         TILE_SIZE, TILE_SIZE,
                                         0);
        }
    }
}

bool TetrisController::Hold() {
    if (falling == nullptr || last_hold)
        return false;

    Tile to_hold = falling->type;
    std::swap(hold, to_hold);

    delete falling;
    falling = nullptr;

    if (to_hold == Tile::NONE)
        NextTetromino();
    else {
        falling = new Tetromino(to_hold, board);
        if (!falling->Success())
            state = TetrisState::LOSE;
    }

    last_hold = true;
    return true;
}

bool TetrisController::Rotate(bool ccw) {
    if (falling == nullptr)
        return false;
    bool success = falling->Rotate(ccw);
    if (!success)
        return false;

    if (remaining_regret_times > 0 && state == TetrisState::LANDING) {
        remaining_regret_times--;
        state = TetrisState::FALLING;
    }

    return true;
}

bool TetrisController::Move(bool left) {
    if (falling == nullptr)
        return false;
    bool success = falling->Move(left);
    if (!success)
        return false;

    if (remaining_regret_times > 0 && state == TetrisState::LANDING) {
        remaining_regret_times--;
        state = TetrisState::FALLING;
    }

    return true;
}

void TetrisController::Fall() {
    if (state != TetrisState::FALLING || falling == nullptr)
        return;

    falling->Fall();

    if (!falling->CanFall()) {
        if (remaining_regret_times <= 0)
            Place();
        else
            state = TetrisState::LANDING;
    }
}

void TetrisController::HardFall() {
    if (state == TetrisState::FALLING) {
        falling->HardFall();
        Place();
    }
}

TetrisState TetrisController::Next() {
    if (state == TetrisState::LANDED) {
        NextTetromino();
    } else if (state == TetrisState::FALLING) {
        Fall();
    } else if (state == TetrisState::LANDING) {
        if (falling->CanFall()) {
            state = TetrisState::FALLING;
            remaining_regret_times--;
            Fall();
        } else {
            Place();
        }
    }
    return state;
}

void TetrisController::NextTetromino() {
    Tile next = next_queue.front();
    next_queue.pop();
    next_queue.emplace(Tile(randint(1, 7)));

    falling = new Tetromino(next, board);
    if (!falling->Success()) {
        state = TetrisState::LOSE;
        return;
    }

    remaining_regret_times = LANDING_REGRET_TIMES;

    last_hold = false;
    state = TetrisState::FALLING;
}

void TetrisController::Place() {
    falling->Place();
    delete falling;
    falling = nullptr;
    state = TetrisState::LANDED;
    CheckLines();

    if (state != TetrisState::LOSE)
        NextTetromino();
}


void TetrisController::CheckLines() {
    for (int i = 0; i < board.size(); i++) {
        if (std::any_of(board[i].begin(), board[i].end(),
                        [](Tile &t){return t == Tile::NONE;}))
            continue;
        ClearLine(i);
        i--;
    }
    for (int i = TILE_COUNT_V; i < board.size(); i++) {
        if (std::any_of(board[i].begin(), board[i].end(),
                        [](Tile &t){return t != Tile::NONE;})) {
            state = TetrisState::LOSE;
        }
    }
}

void TetrisController::ClearLine(int y) {
    board.erase(board.begin() + y);
    board.emplace_back(std::vector<Tile>(TILE_COUNT_H, Tile::NONE));
}
