#pragma once

#include "log.h"
#include "tetriscontroller.h"

#include <globals.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

class Game {
 public:
    Game(GameType, ALLEGRO_DISPLAY*, ALLEGRO_TIMER*);
    ~Game();

    GameResult Start();



 private:
    static void drawBackground();
    void updateScreen();

    ALLEGRO_TIMER *fall;
    ALLEGRO_EVENT_QUEUE *eventQueue;

    TetrisController *tc;

    GameType gameType;
};