#pragma once

#include "log.h"
#include "tetriscontroller.h"

#include "server.h"
#include "client.h"

class Server;
class Client;

class TetrisController;

#include <thread>
#include "globals.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

class Game {
 public:
    Game(GameType, ALLEGRO_DISPLAY*, ALLEGRO_TIMER*);
    ~Game();

    GameResult Start();

    void StartGame();

    Server *server = nullptr;
    Client *client = nullptr;
    GameStatus status = GameStatus::PENDING;
    bool is_multi = false;



 private:
    static void drawBackground();
    void drawMulti() const;
    void updateScreen();

    void handleKeyPress(int keycode);

    ALLEGRO_TIMER *fall;
    ALLEGRO_TIMER *das;
    ALLEGRO_EVENT_QUEUE *eventQueue;

    TetrisController *tc;

    GameType gameType;


    std::thread server_thread;
    std::thread client_thread;

};