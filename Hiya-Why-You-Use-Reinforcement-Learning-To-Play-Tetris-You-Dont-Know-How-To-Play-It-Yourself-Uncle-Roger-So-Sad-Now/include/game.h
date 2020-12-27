#pragma once

#include "log.h"

#include <globals.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

class Game {
 public:
    Game(GameType, ALLEGRO_DISPLAY*, ALLEGRO_TIMER*);
    ~Game();

    GameResult Start();

    static const int TILE_COUNT_H = 10;
    static const int TILE_COUNT_V = 20;
    static const int GIRD_WIDTH = 2;
    static const int TILE_SIZE = 40 + GIRD_WIDTH;
    static const int GAMEPLAY_X = 300;
    static const int GAMEPLAY_Y = (WINDOW_HEIGHT - TILE_SIZE * TILE_COUNT_V) / 2;
    static const int GAMEPLAY_WIDTH = TILE_SIZE * TILE_COUNT_H;
    static const int GAMEPLAY_HEIGHT = TILE_SIZE * TILE_COUNT_V;
    static const int HOLDAREA_SIZE;
    static const int BORDER_OUTER_WIDTH = 10;
    static const int BORDER_INNER_WIDTH = 5;
    static const int PREVIEW_WIDTH;
    static const int PREVIEW_HEIGHT;


 private:
    static void drawBackground();
    static void updateScreen();

    ALLEGRO_EVENT_QUEUE *eventQueue;

    GameType gameType;
};