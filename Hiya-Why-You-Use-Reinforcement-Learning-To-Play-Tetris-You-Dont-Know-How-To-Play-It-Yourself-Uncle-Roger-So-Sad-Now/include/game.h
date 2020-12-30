#pragma once

#include "log.h"

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
    static void updateScreen();

    ALLEGRO_EVENT_QUEUE *eventQueue;

    GameType gameType;
};