#pragma once

#include <allegro5/allegro_font.h>
#include <queue>
#include <vector>
#include <random>

namespace Constants {

static const int FPS = 60;
static const int WINDOW_WIDTH = 1600;
static const int WINDOW_HEIGHT = 900;

static const double FALL_TIME = 0.75;

static const int TILE_COUNT_H = 10;
static const int TILE_COUNT_V = 20;

static const int GIRD_WIDTH = 2;
static const int TILE_SIZE = 40 + GIRD_WIDTH;
static const int MINI_TILE_SIZE = TILE_SIZE * 0.6;

static const int GAMEPLAY_WIDTH = TILE_SIZE * TILE_COUNT_H;
static const int GAMEPLAY_HEIGHT = TILE_SIZE * TILE_COUNT_V;

// Top left of the gameplay area
static const int GAMEPLAY_X = (WINDOW_WIDTH - GAMEPLAY_WIDTH)/2;
static const int GAMEPLAY_Y = (WINDOW_HEIGHT - GAMEPLAY_HEIGHT) / 2;


static const int BORDER_OUTER_WIDTH = 10;
static const int BORDER_INNER_WIDTH = 5;

static const int HOLDAREA_SIZE = 3 * TILE_SIZE;
// Top left of the hold area
static const int HOLDAREA_X = GAMEPLAY_X - (2*BORDER_OUTER_WIDTH - (BORDER_OUTER_WIDTH - BORDER_INNER_WIDTH)) - HOLDAREA_SIZE;
static const int HOLDAREA_Y = GAMEPLAY_Y;

static const int GARBAGE_BUFFER_TILE_COUNT = 16;
static const int GARBAGE_BUFFER_X = GAMEPLAY_X - (2*BORDER_OUTER_WIDTH - (BORDER_OUTER_WIDTH - BORDER_INNER_WIDTH)) - TILE_SIZE;
static const int GARBAGE_BUFFER_Y = GAMEPLAY_Y + HOLDAREA_SIZE + (2*BORDER_OUTER_WIDTH - (BORDER_OUTER_WIDTH - BORDER_INNER_WIDTH));
static const int GARBAGE_BUFFER_WIDTH = TILE_SIZE;
static const int GARBAGE_BUFFER_HEIGHT = GAMEPLAY_HEIGHT - HOLDAREA_SIZE - (2*BORDER_OUTER_WIDTH - (BORDER_OUTER_WIDTH - BORDER_INNER_WIDTH));



static const int PREVIEW_COUNT = 5;
static const int PREVIEW_AREA_X = GAMEPLAY_X + GAMEPLAY_WIDTH + (2*BORDER_OUTER_WIDTH - (BORDER_OUTER_WIDTH - BORDER_INNER_WIDTH));
static const int PREVIEW_AREA_Y = GAMEPLAY_Y;
static const int PREVIEW_AREA_WIDTH = MINI_TILE_SIZE * PREVIEW_COUNT;
static const int PREVIEW_AREA_HEIGHT = MINI_TILE_SIZE * 4 * PREVIEW_COUNT + MINI_TILE_SIZE;

static const double DAS_HOLD_SECONDS = 0.2;
static const double DAS_INTERVAL_SECONDS = 0.06;

static const int LANDING_REGRET_TIMES = 5;

static const double CLEAR_LINE_ANIMATION_S1 = 0.05;
static const double CLEAR_LINE_ANIMATION_S2 = 0.12;
static const double CLEAR_LINE_ANIMATION_END = 0.3;
static const double CLEAR_LINE_ANIMATION_ALPHA = 1.0;

static const double DEATH_ANIMATION_INTERVAL = 0.1;

static ALLEGRO_COLOR BACKGROUND_COLOR;
static ALLEGRO_COLOR TEXT_COLOR;
static ALLEGRO_COLOR GAMEPLAY_BG_COLOR;
static ALLEGRO_COLOR GIRD_COLOR;
static ALLEGRO_COLOR BORDER_OUTER_COLOR;
static ALLEGRO_COLOR BORDER_INNER_COLOR;



static const int TETROMINO_BLOCK_TEXTURE_SIZE = 45;


static const double TETROMINO_GHOST_ALPHA = 0.3;
static const double TETROMINO_BOARD_ALPHA = 0.75;


static const int SERVER_PORT = 7122;

static const int BUFFER_SIZE = 5000;

inline void init_colors() {
    BACKGROUND_COLOR = al_map_rgb(210, 210, 210);
    TEXT_COLOR = al_map_rgb(210, 210, 210);
    GAMEPLAY_BG_COLOR = al_map_rgb(57, 57, 57);
    GIRD_COLOR = al_map_rgb(47, 47, 47);
    BORDER_OUTER_COLOR = al_map_rgb(58, 219, 209);
    BORDER_INNER_COLOR = al_map_rgb(36, 133, 122);
}

}


inline int randint(int from, int to) {
    std::random_device rd;
    auto gen = std::mt19937(rd());
    auto dis = std::uniform_int_distribution<int>(from, to);
    return dis(gen);
}


inline void al_reset_timer(ALLEGRO_TIMER  *timer) {
    al_stop_timer(timer);
    al_start_timer(timer);
}

enum class GameStatus {
  MENU, PENDING, PLAYING, DEAD
};

enum class GameType {
    SINGLE, MULTI_HOST, MULTI_CLIENT, EXIT
};

enum class GameResult {
    EXIT, WIN, LOSE
};

enum class TetrisState {
    FALLING, LANDING, LANDED, LOSE
};

enum class Tile {
    NONE,
    RED, // Z Block
    ORANGE, // L Block
    YELLOW, // O Block
    GREEN, // S Block
    BLUE, // J Block
    SKY, // I Block
    PURPLE, // T Block
    GRAY // Garbage Blocks
};

enum class HiyaOperation {
    REGISTER = 1, PLAYERS, START, UPDATE_BOARD, ATTACK, DEATH
};

struct Position {
    int x;
    int y;
};

using Board = std::deque<std::vector<Tile>>;
