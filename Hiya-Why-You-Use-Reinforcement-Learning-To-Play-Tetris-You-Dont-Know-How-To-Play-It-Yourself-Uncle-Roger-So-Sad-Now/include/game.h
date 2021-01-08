#pragma once

#include "log.h"
#include "tetriscontroller.h"

#include "server.h"
#include "client.h"

class Server;
class Client;

#include <globals.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

class Game {
 public:
    Game(GameType, ALLEGRO_DISPLAY*, ALLEGRO_TIMER*);
    ~Game();

    GameResult Start();

    Server *server = nullptr;
    Client *client = nullptr;
    GameStatus status = GameStatus::PENDING;


 private:
    static void drawBackground();
    void updateScreen();

    void handleKeyPress(int keycode);

    ALLEGRO_TIMER *fall;
    ALLEGRO_TIMER *das;
    ALLEGRO_EVENT_QUEUE *eventQueue;

    TetrisController *tc;

    GameType gameType;

    bool is_multi = false;

};