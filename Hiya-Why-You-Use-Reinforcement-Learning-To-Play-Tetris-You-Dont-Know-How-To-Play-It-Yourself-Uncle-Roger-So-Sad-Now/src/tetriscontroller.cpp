#include "tetriscontroller.h"

#include <allegro5/allegro_primitives.h>
#include <random>
#include "log.h"

#include "animations.h"
#include "window.h"

bool TetrisController::textures_loaded = false;
ALLEGRO_BITMAP *TetrisController::tetrimino_textures[10];
ALLEGRO_BITMAP *TetrisController::hold_text;
ALLEGRO_BITMAP *TetrisController::next_text;

TetrisController::TetrisController(ALLEGRO_TIMER *fall, Game &game): state(TetrisState::LANDED), remaining_regret_times(LANDING_REGRET_TIMES), fall(fall), game(game) {
    for (int i = 0; i < TILE_COUNT_V + 5; i++)
        board.emplace_back(std::vector<Tile>(TILE_COUNT_H, Tile::NONE));

    for (int i = 0; i < PREVIEW_COUNT; i++)
        next_queue.emplace_back(Tile(randint(1, 7)));

    if (!textures_loaded) {
        tetrimino_textures[int(Tile::RED)] = al_load_bitmap("../assets/block-red.png");
        tetrimino_textures[int(Tile::ORANGE)] = al_load_bitmap("../assets/block-orange.png");
        tetrimino_textures[int(Tile::YELLOW)] = al_load_bitmap("../assets/block-yellow.png");
        tetrimino_textures[int(Tile::GREEN)] = al_load_bitmap("../assets/block-green.png");
        tetrimino_textures[int(Tile::BLUE)] = al_load_bitmap("../assets/block-blue.png");
        tetrimino_textures[int(Tile::SKY)] = al_load_bitmap("../assets/block-sky.png");
        tetrimino_textures[int(Tile::PURPLE)] = al_load_bitmap("../assets/block-purple.png");
        tetrimino_textures[int(Tile::GRAY)] = al_load_bitmap("../assets/block-gray.png");
        tetrimino_textures[int(Tile::LAVA)] = al_load_bitmap("../assets/block-lava.png");

        hold_text = al_load_bitmap("../assets/hold-text.png");
        next_text = al_load_bitmap("../assets/next-text.png");

        textures_loaded = true;
    }
}

void TetrisController::Draw() {
    if (falling != nullptr)
        falling->Draw();

    /// Board
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


    /// Hold
    al_draw_scaled_bitmap(hold_text, 0, 0,
                          al_get_bitmap_width(hold_text), al_get_bitmap_height(hold_text),
                          HOLDAREA_X + MINI_TILE_SIZE/2.0, HOLDAREA_Y,
                          HOLDAREA_SIZE - MINI_TILE_SIZE, (HOLDAREA_SIZE - MINI_TILE_SIZE)/2.0,
                          0);
    if (hold != Tile::NONE) {
        const int block_size = Tetromino::block_sizes[int(hold)];
        const auto block = Tetromino::block_types[int(hold)];
        for (int i = 0; i < block_size; i++) {
            for (int j = 0; j < block_size; j++) {
                if (block[i][j] != Tile::NONE) {
                    al_draw_scaled_bitmap(tetrimino_textures[int(block[i][j])],
                                          0, 0, TETROMINO_BLOCK_TEXTURE_SIZE, TETROMINO_BLOCK_TEXTURE_SIZE,
                                          HOLDAREA_X + (HOLDAREA_SIZE - block_size * MINI_TILE_SIZE) / 2.0 + j * MINI_TILE_SIZE,
                                          HOLDAREA_Y + MINI_TILE_SIZE + (HOLDAREA_SIZE - block_size * MINI_TILE_SIZE) / 2.0 + i * MINI_TILE_SIZE,
                                          MINI_TILE_SIZE, MINI_TILE_SIZE,
                                          0);
                }
            }
        }
    }

    /// Next
    al_draw_scaled_bitmap(next_text, 0, 0,
                          al_get_bitmap_width(next_text), al_get_bitmap_height(next_text),
                          PREVIEW_AREA_X + MINI_TILE_SIZE/2.0, PREVIEW_AREA_Y,
                          PREVIEW_AREA_WIDTH - MINI_TILE_SIZE, (PREVIEW_AREA_WIDTH - MINI_TILE_SIZE)/2.0,
                          0);
    for (int p = 0; p < PREVIEW_COUNT; p++) {
        const int block_size = Tetromino::block_sizes[int(next_queue[p])];
        const auto block = Tetromino::block_types[int(next_queue[p])];
        for (int i = 0; i < block_size; i++) {
            for (int j = 0; j < block_size; j++) {
                if (block[i][j] != Tile::NONE) {
                    al_draw_scaled_bitmap(tetrimino_textures[int(block[i][j])],
                                          0, 0, TETROMINO_BLOCK_TEXTURE_SIZE, TETROMINO_BLOCK_TEXTURE_SIZE,
                                          PREVIEW_AREA_X + (PREVIEW_AREA_WIDTH - block_size * MINI_TILE_SIZE) / 2.0 + j * MINI_TILE_SIZE,
                                          PREVIEW_AREA_Y + MINI_TILE_SIZE + (PREVIEW_AREA_WIDTH - block_size * MINI_TILE_SIZE) / 2.0 + i * MINI_TILE_SIZE + p * MINI_TILE_SIZE * 4,
                                          MINI_TILE_SIZE, MINI_TILE_SIZE,
                                          0);
                }
            }
        }
    }

    // Garbage
    if (game.status == GameStatus::PLAYING && !dying) {
        CheckGarbage();

        int last = 0;
        double cur_time = al_get_time();
        for (const auto &[lines, time] : garbage_buffer) {
            Tile garbage;
            const double diff_sec = cur_time - time;
            if (diff_sec < GARBAGE_BUFFER_S1_SEC)
                garbage = Tile::GRAY;
            else if (diff_sec < GARBAGE_BUFFER_S2_SEC)
                garbage = Tile::YELLOW;
            else if (diff_sec < GARBAGE_BUFFER_S3_SEC)
                garbage = Tile::RED;
            else {
                if (fmod(diff_sec, GARBAGE_BUFFER_LAVA_FLASH_INTERVAL * 2) < GARBAGE_BUFFER_LAVA_FLASH_INTERVAL)
                    garbage = Tile::RED;
                else
                    garbage = Tile::LAVA;
            }

            for (int i = 0; i < lines && last + i < GARBAGE_BUFFER_TILE_COUNT; i++) {
                al_draw_scaled_bitmap(tetrimino_textures[int(garbage)],
                                      0, 0, TETROMINO_BLOCK_TEXTURE_SIZE, TETROMINO_BLOCK_TEXTURE_SIZE,
                                      GARBAGE_BUFFER_X, GARBAGE_BUFFER_Y + GARBAGE_BUFFER_HEIGHT - (last + i + 1) * TILE_SIZE,
                                      TILE_SIZE, TILE_SIZE,
                                      0);
            }
            last += lines;
        }
    }


    if (clearing_line)
        ClearLines();

    if (dying)
        Dying();

}

bool TetrisController::Hold() {
    if (falling == nullptr || last_hold)
        return false;

    Tile to_hold = falling->type;
    std::swap(hold, to_hold);

    delete falling;
    falling = nullptr;

    al_play_sample(Window::se_hold, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);

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
    al_play_sample(Window::se_move, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);

    return true;
}

bool TetrisController::Move(bool left) {
    if (falling == nullptr)
        return false;
    bool success = falling->Move(left);
    if (!success)
        return false;
    al_play_sample(Window::se_move, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);

    if (remaining_regret_times > 0 && state == TetrisState::LANDING) {
        remaining_regret_times--;
        state = TetrisState::FALLING;
    }

    return true;
}

void TetrisController::Fall() {
    if (state != TetrisState::FALLING || falling == nullptr)
        return;

    al_reset_timer(fall);
    falling->Fall();

    if (!falling->CanFall()) {
        if (remaining_regret_times <= 0)
            Place();
        else
            state = TetrisState::LANDING;
    }
}

void TetrisController::HardFall() {
    if (state == TetrisState::FALLING || state == TetrisState::LANDING) {
        falling->HardFall();
        al_play_sample(Window::se_harddrop, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
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
    next_queue.pop_front();
    next_queue.emplace_back(Tile(randint(1, 7)));

    falling = new Tetromino(next, board);
    if (!falling->Success()) {
        state = TetrisState::LOSE;
        return;
    }

    remaining_regret_times = LANDING_REGRET_TIMES;

    last_hold = false;
    state = TetrisState::FALLING;
    al_reset_timer(fall);
}

void TetrisController::Place() {
    falling->Place();
    delete falling;
    falling = nullptr;
//    state = TetrisState::LANDED;
    if (game.is_multi) {
        game.client->SendUpdateBoard(board);
    }
    CheckLines();

    if (!clearing_line && state != TetrisState::LOSE)
        NextTetromino();
}


void TetrisController::CheckLines() {
    for (int i = board.size() - 1; i >= 0; i--) {
        if (std::any_of(board[i].begin(), board[i].end(),
                        [](Tile &t){return t == Tile::NONE;}))
            continue;
        lines_to_clear.emplace_back(i);
    }

    if (!lines_to_clear.empty()) {
        combo++;
        ClearLines();
    } else {
        combo = 0;
        CheckDeath();
    }
}

void TetrisController::ClearLines() {
    static std::vector<ClearLineAnimation*> animations;
    if (!clearing_line) { // Init
        clearing_line = true;
        animations.clear();
        for (int y: lines_to_clear) {
            board[y].assign(TILE_COUNT_H, Tile::NONE);
            animations.emplace_back(new ClearLineAnimation(y));
        }

        if (lines_to_clear.size() == 1)
            al_play_sample(Window::se_single, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
        else if (lines_to_clear.size() == 2)
            al_play_sample(Window::se_double, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
        else if (lines_to_clear.size() == 3)
            al_play_sample(Window::se_triple, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
        else if (lines_to_clear.size() >= 4)
            al_play_sample(Window::se_tetris, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);

        if (game.is_multi && !game.client->players_alive.empty()
             && (lines_to_clear.size() >= 2 || combo >= 2 || !garbage_buffer.empty())) {
            int n = randint(0, game.client->players_alive.size() - 1);
            auto it = game.client->players_alive.begin();
            while (n--)
                it++;
            const int target = *it;
            const int nth = std::find(game.client->player_list.begin(), game.client->player_list.end(), target) - game.client->player_list.begin();


            const int lines = lines_to_clear.size() + combo - 1;

            int garbage = 0;
            while (!garbage_buffer.empty() && garbage < lines) {
                auto &[ls, time] = garbage_buffer.front();
                if (lines - garbage > ls) {
                    garbage += ls;
                    garbage_buffer.pop_front();
                } else {
                    ls -= lines - garbage;
                    garbage = lines;
                }
            }

            const int sx = GAMEPLAY_X + GAMEPLAY_WIDTH/2;
            const int sy = GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - lines_to_clear[lines_to_clear.size()/2] - 1);
            const int dx = MULTI_X[nth] + MULTI_WIDTH/2;
            const int dy = MULTI_Y[nth] + MULTI_HEIGHT/2;
            game.send_line_animations.emplace_back(new SendLineAnimation(sx, sy, dx, dy));

            game.client->SendAttack(target, lines + (garbage? 1: 0));
        }
    }

    bool finished = true;
    for (auto &ani: animations) {
        if (ani == nullptr)
            continue;
        finished = false;
        if (ani->NextFrame()) {
            delete ani;
            ani = nullptr;
        }
    }

    if (finished) {
        for (int y: lines_to_clear) {
            board.erase(board.begin() + y);
            board.emplace_back(std::vector<Tile>(TILE_COUNT_H, Tile::NONE));
        }

        CheckDeath();

        if (state != TetrisState::LOSE)
            NextTetromino();

        lines_to_clear.clear();
        clearing_line = false;
    }

}

void TetrisController::CheckDeath() {
    for (int i = TILE_COUNT_V; i < board.size(); i++) {
        if (std::any_of(board[i].begin(), board[i].end(),
                        [](Tile &t){return t != Tile::NONE;})) {
            state = TetrisState::LOSE;
            Dying();
            return;
        }
    }
}

void TetrisController::Dying() {
    static std::vector<ClearLineAnimation*> animations;
    static int y;
    static double last_time;

    if (!dying) { // Init
        dying = true;
        animations.clear();
        y = 0;
        last_time = 0;
        if (game.is_multi)
            game.client->SendDead();

        al_stop_sample(&Window::gameplay_sampid);
        al_play_sample(Window::me_lose, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }

    if (y < TILE_COUNT_V && al_get_time() - last_time >= DEATH_ANIMATION_INTERVAL) {
        last_time = al_get_time();

        board[y].assign(TILE_COUNT_H, Tile::NONE);
        animations.push_back(new ClearLineAnimation(y));
        y++;
    }

    bool finished = true;
    for (auto &ani: animations) {
        if (ani == nullptr)
            continue;
        finished = false;
        if (ani->NextFrame()) {
            delete ani;
            ani = nullptr;
        }
    }

    if (finished) {
        dying = false;
//        garbage_buffer.clear();

        if (game.is_multi)
            game.EndGame(GameResult::LOSE, game.client->players_alive.size() + 1);
        else
            game.EndGame(GameResult::LOSE);
    }
}

void TetrisController::ReceiveAttack(int lines) {
    garbage_buffer.emplace_back(lines, al_get_time());
}

void TetrisController::CheckGarbage() {
    if (clearing_line)
        return;

    double cur_time = al_get_time();
    while (!garbage_buffer.empty()) {
        auto [lines, time] = *garbage_buffer.begin();
        if (cur_time - time >= GARBAGE_BUFFER_PUSH_SEC) {
            const int hole = randint(0, TILE_COUNT_H - 1);
            auto line = std::vector<Tile>(TILE_COUNT_H, Tile::GRAY);
            line[hole] = Tile::NONE;
            if (falling != nullptr)
                falling->y += lines;

            while (lines--)
                board.emplace_front(line);

            garbage_buffer.pop_front();
        } else {
            break;
        }
    }
}
