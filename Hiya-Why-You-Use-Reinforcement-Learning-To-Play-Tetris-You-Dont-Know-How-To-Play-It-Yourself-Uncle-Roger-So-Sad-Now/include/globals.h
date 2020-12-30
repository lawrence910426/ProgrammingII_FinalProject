#pragma once

#include <allegro5/allegro_font.h>
#include <queue>
#include <vector>


namespace Constants {

static const int FPS = 60;
static const int WINDOW_WIDTH = 1600;
static const int WINDOW_HEIGHT = 900;


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


static const int PREVIEW_AREA_X = GAMEPLAY_X + GAMEPLAY_WIDTH + (2*BORDER_OUTER_WIDTH - (BORDER_OUTER_WIDTH - BORDER_INNER_WIDTH));
static const int PREVIEW_AREA_Y = GAMEPLAY_Y;
static const int PREVIEW_AREA_WIDTH = MINI_TILE_SIZE * 5;
static const int PREVIEW_AREA_HEIGHT = MINI_TILE_SIZE * 4 * 5;


static ALLEGRO_COLOR BACKGROUND_COLOR;
static ALLEGRO_COLOR GAMEPLAY_BG_COLOR;
static ALLEGRO_COLOR GIRD_COLOR;
static ALLEGRO_COLOR BORDER_OUTER_COLOR;
static ALLEGRO_COLOR BORDER_INNER_COLOR;

//
//    static const int PREVIEW_WIDTH;
//    static const int PREVIEW_HEIGHT;

inline void init_colors() {
    BACKGROUND_COLOR = al_map_rgb(210, 210, 210);
    GAMEPLAY_BG_COLOR = al_map_rgb(57, 57, 57);
    GIRD_COLOR = al_map_rgb(47, 47, 47);
    BORDER_OUTER_COLOR = al_map_rgb(58, 219, 209);
    BORDER_INNER_COLOR = al_map_rgb(36, 133, 122);
}

}




enum class GameStatus {
  MENU, CONNECTING, LOADING, PLAYING, ENDING, EXIT
};

extern GameStatus gameStatus;

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
    NONE, RED, ORANGE, YELLOW, GREEN, BLUE, SKY, PURPLE
};

struct Position {
    int x;
    int y;
};

using Board = std::deque<std::vector<Tile>>;
