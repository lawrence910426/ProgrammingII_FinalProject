#include "game.h"

using namespace Constants;

#include <map>
#include <thread>
#include "window.h"

bool Game::textures_loaded = false;

void server_process(Server *server) {
    while (server && server->running)
        server->handle();
}

void client_process(Client *client) {
    while (client && client->running)
        client->handle();
}

//void *server_process(ALLEGRO_THREAD *t, void *arg) {
//    auto *server = (Server *)arg;
//    while (server->running) {
//        server->handle();
//        usleep(1000);
//    }
//    delete server;
//
//    return nullptr;
//}
//
//void *client_process(ALLEGRO_THREAD *t, void *arg) {
//    auto *client = (Client *) arg;
//    while (client->running){
//        client->handle();
//        usleep(1000);
//    }
//    delete client;
//
//    return nullptr;
//}

Game::Game(GameType type, ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *tick) {
    eventQueue = al_create_event_queue();
    if (!eventQueue)
        FATAL("Failed to create event queue!");

    if (!al_init_primitives_addon())
        FATAL("Primitives addon init failed!");

    fall = al_create_timer(FALL_TIME);
    if (!fall)
        FATAL("Fall timer create failed");

    das = al_create_timer(DAS_INTERVAL_SECONDS);
    if (!das)
        FATAL("DAS timer create failed");

    init_colors();

    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(tick));
    al_register_event_source(eventQueue, al_get_timer_event_source(fall));
    al_register_event_source(eventQueue, al_get_timer_event_source(das));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    gameType = type;
    is_multi = (type == GameType::MULTI_HOST || type == GameType::MULTI_CLIENT);

    if (type == GameType::MULTI_HOST) {
        server = new Server(SERVER_PORT, *this);
        //        ALLEGRO_THREAD *server_thread = al_create_thread(server_process, server);
        //        al_start_thread(server_thread);
        server_thread = std::thread(server_process, server);
    }
    //
    if (is_multi) {
        if (type == GameType::MULTI_HOST)
            client = new Client(server->master_fd, *this);
        else {
            char host[] = "dorm.yikuo.dev";
            client = new Client(host, 7122, *this);
        }
        //        ALLEGRO_THREAD *client_thread = al_create_thread(client_process, client);
        //        al_start_thread(client_thread);
        client_thread = std::thread(client_process, client);
    }

    tc = new TetrisController(fall, *this);

}

Game::~Game() {
    al_destroy_event_queue(eventQueue);
    delete tc;

    if (client != nullptr) {
        delete client;
        client_thread.detach();
    }

    if (server != nullptr) {
        delete server;
        server_thread.detach();
    }
}

GameResult Game::Start() {
    ALLEGRO_EVENT event;

    std::string name = "HelloYi";
    if (is_multi)
        client->SendRegister(name);

    // keycode -> holding, last_hold_time
    std::map<int, std::pair<bool, double>> das_map;


    for (;;) {
        al_wait_for_event(eventQueue, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                return GameResult::EXIT;

            case ALLEGRO_EVENT_KEY_DOWN:
                handleKeyPress(event.keyboard.keycode);

                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_LEFT:
                    case ALLEGRO_KEY_RIGHT:
                    case ALLEGRO_KEY_DOWN:
                        das_map[event.keyboard.keycode] = {true, al_get_time()};
                }
                break;

            case ALLEGRO_EVENT_KEY_UP:
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_LEFT:
                    case ALLEGRO_KEY_RIGHT:
                    case ALLEGRO_KEY_DOWN:
                        das_map[event.keyboard.keycode].first = false;
                }
                break;

            case ALLEGRO_EVENT_TIMER:
                if (event.timer.source == fall) {
                    tc->Next();
                } else if (event.timer.source == das) {
                    for (auto &[keycode, val] : das_map) {
                        auto &[holding, last_hold_time] = val;
                        if (holding) {
                            if (al_get_time() - last_hold_time >= DAS_HOLD_SECONDS) {
                                handleKeyPress(keycode);
                            }
                        }
                    }
                } else {// tick
                    updateScreen();
                }
                break;
        }
    }
}

void Game::handleKeyPress(int keycode) {
    if (status == GameStatus::PLAYING) {
        if (keycode == ALLEGRO_KEY_RIGHT)
            tc->Move(false);
        else if (keycode == ALLEGRO_KEY_LEFT)
            tc->Move(true);
        else if (keycode == ALLEGRO_KEY_UP)
            tc->Rotate(false);
        else if (keycode == ALLEGRO_KEY_DOWN)
            tc->Fall();
        else if (keycode == ALLEGRO_KEY_SPACE)
            tc->HardFall();
        else if (keycode == ALLEGRO_KEY_C)
            tc->Hold();
        else if (keycode == ALLEGRO_KEY_Z)
            tc->Rotate(true);
    } else if (status == GameStatus::PENDING) {
        if (keycode == ALLEGRO_KEY_ENTER) {
            if (gameType == GameType::MULTI_HOST)
                server->SendGameStart();
            else if (gameType == GameType::SINGLE)
                StartGame();
        }
    }
}

void Game::StartGame() {
    al_start_timer(fall);
    al_start_timer(das);
    status = GameStatus::PLAYING;
}

void Game::updateScreen() {
    drawBackground();
    if (status == GameStatus::PLAYING)
        tc->Draw();

    if (is_multi && client->id != -1)
        drawMulti();

    drawTexts();

    al_flip_display();
}

void Game::drawBackground() {

    /// Background
    al_draw_filled_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, BACKGROUND_COLOR);

    /// Gameplay Area
    // Background
    al_draw_filled_rectangle(GAMEPLAY_X, GAMEPLAY_Y,
                             GAMEPLAY_X + GAMEPLAY_WIDTH, GAMEPLAY_Y + GAMEPLAY_HEIGHT,
                             GAMEPLAY_BG_COLOR);
    // Girds
    for (int i = 0; i < TILE_COUNT_V; i++)
        al_draw_line(GAMEPLAY_X, GAMEPLAY_Y + i * TILE_SIZE,
                     GAMEPLAY_X + GAMEPLAY_WIDTH, GAMEPLAY_Y + i * TILE_SIZE,
                     GIRD_COLOR, GIRD_WIDTH);
    for (int i = 0; i < TILE_COUNT_H; i++)
        al_draw_line(GAMEPLAY_X + i * TILE_SIZE, GAMEPLAY_Y,
                     GAMEPLAY_X + i * TILE_SIZE, GAMEPLAY_Y + GAMEPLAY_HEIGHT,
                     GIRD_COLOR, GIRD_WIDTH);
    // Border
    al_draw_rounded_rectangle(GAMEPLAY_X - BORDER_OUTER_WIDTH / 2.0, GAMEPLAY_Y - BORDER_OUTER_WIDTH / 2.0,
                              GAMEPLAY_X + GAMEPLAY_WIDTH + BORDER_OUTER_WIDTH / 2.0, GAMEPLAY_Y + GAMEPLAY_HEIGHT + BORDER_OUTER_WIDTH / 2.0,
                              BORDER_OUTER_WIDTH / 2.0, BORDER_OUTER_WIDTH / 2.0,
                              BORDER_OUTER_COLOR, BORDER_OUTER_WIDTH);
    al_draw_rounded_rectangle(GAMEPLAY_X - BORDER_INNER_WIDTH / 2.0, GAMEPLAY_Y - BORDER_INNER_WIDTH / 2.0,
                              GAMEPLAY_X + GAMEPLAY_WIDTH + BORDER_INNER_WIDTH / 2.0, GAMEPLAY_Y + GAMEPLAY_HEIGHT + BORDER_INNER_WIDTH / 2.0,
                              BORDER_INNER_WIDTH / 2.0, BORDER_INNER_WIDTH / 2.0,
                              BORDER_INNER_COLOR, BORDER_INNER_WIDTH);

    /// Hold Area
    // Background
    al_draw_filled_rectangle(HOLDAREA_X, HOLDAREA_Y,
                             HOLDAREA_X + HOLDAREA_SIZE, HOLDAREA_Y + HOLDAREA_SIZE,
                             GAMEPLAY_BG_COLOR);
    // Border
    al_draw_rounded_rectangle(HOLDAREA_X - BORDER_OUTER_WIDTH / 2.0, HOLDAREA_Y - BORDER_OUTER_WIDTH / 2.0,
                              HOLDAREA_X + HOLDAREA_SIZE + BORDER_OUTER_WIDTH / 2.0, HOLDAREA_Y + HOLDAREA_SIZE + BORDER_OUTER_WIDTH / 2.0,
                              BORDER_OUTER_WIDTH / 2.0, BORDER_OUTER_WIDTH / 2.0,
                              BORDER_OUTER_COLOR, BORDER_OUTER_WIDTH);
    al_draw_rounded_rectangle(HOLDAREA_X - BORDER_INNER_WIDTH / 2.0, HOLDAREA_Y - BORDER_INNER_WIDTH / 2.0,
                              HOLDAREA_X + HOLDAREA_SIZE + BORDER_INNER_WIDTH / 2.0, HOLDAREA_Y + HOLDAREA_SIZE + BORDER_INNER_WIDTH / 2.0,
                              BORDER_INNER_WIDTH / 2.0, BORDER_INNER_WIDTH / 2.0,
                              BORDER_INNER_COLOR, BORDER_INNER_WIDTH);

    /// Garbage buffer
    // Background
    al_draw_filled_rectangle(GARBAGE_BUFFER_X, GARBAGE_BUFFER_Y,
                             GARBAGE_BUFFER_X + GARBAGE_BUFFER_WIDTH, GARBAGE_BUFFER_Y + GARBAGE_BUFFER_HEIGHT,
                             GAMEPLAY_BG_COLOR);
    // Border
    al_draw_rounded_rectangle(GARBAGE_BUFFER_X - BORDER_OUTER_WIDTH / 2.0, GARBAGE_BUFFER_Y - BORDER_OUTER_WIDTH / 2.0,
                              GARBAGE_BUFFER_X + GARBAGE_BUFFER_WIDTH + BORDER_OUTER_WIDTH / 2.0, GARBAGE_BUFFER_Y + GARBAGE_BUFFER_HEIGHT + BORDER_OUTER_WIDTH / 2.0,
                              BORDER_OUTER_WIDTH / 2.0, BORDER_OUTER_WIDTH / 2.0,
                              BORDER_OUTER_COLOR, BORDER_OUTER_WIDTH);
    al_draw_rounded_rectangle(GARBAGE_BUFFER_X - BORDER_INNER_WIDTH / 2.0, GARBAGE_BUFFER_Y - BORDER_INNER_WIDTH / 2.0,
                              GARBAGE_BUFFER_X + GARBAGE_BUFFER_WIDTH + BORDER_INNER_WIDTH / 2.0, GARBAGE_BUFFER_Y + GARBAGE_BUFFER_HEIGHT + BORDER_INNER_WIDTH / 2.0,
                              BORDER_INNER_WIDTH / 2.0, BORDER_INNER_WIDTH / 2.0,
                              BORDER_INNER_COLOR, BORDER_INNER_WIDTH);

    //    for (int y = GARBAGE_BUFFER_Y + GARBAGE_BUFFER_HEIGHT; y >= GARBAGE_BUFFER_Y; y -= TILE_SIZE)
    //        al_draw_line(GARBAGE_BUFFER_X, y,
    //                     GARBAGE_BUFFER_X + GARBAGE_BUFFER_WIDTH, y,
    //                     GIRD_COLOR, GIRD_WIDTH);

    /// Preview area
    // Background
    al_draw_filled_rectangle(PREVIEW_AREA_X, PREVIEW_AREA_Y,
                             PREVIEW_AREA_X + PREVIEW_AREA_WIDTH, PREVIEW_AREA_Y + PREVIEW_AREA_HEIGHT,
                             GAMEPLAY_BG_COLOR);
    // Border
    al_draw_rounded_rectangle(PREVIEW_AREA_X - BORDER_OUTER_WIDTH / 2.0, PREVIEW_AREA_Y - BORDER_OUTER_WIDTH / 2.0,
                              PREVIEW_AREA_X + PREVIEW_AREA_WIDTH + BORDER_OUTER_WIDTH / 2.0, PREVIEW_AREA_Y + PREVIEW_AREA_HEIGHT + BORDER_OUTER_WIDTH / 2.0,
                              BORDER_OUTER_WIDTH / 2.0, BORDER_OUTER_WIDTH / 2.0,
                              BORDER_OUTER_COLOR, BORDER_OUTER_WIDTH);
    al_draw_rounded_rectangle(PREVIEW_AREA_X - BORDER_INNER_WIDTH / 2.0, PREVIEW_AREA_Y - BORDER_INNER_WIDTH / 2.0,
                              PREVIEW_AREA_X + PREVIEW_AREA_WIDTH + BORDER_INNER_WIDTH / 2.0, PREVIEW_AREA_Y + PREVIEW_AREA_HEIGHT + BORDER_INNER_WIDTH / 2.0,
                              BORDER_INNER_WIDTH / 2.0, BORDER_INNER_WIDTH / 2.0,
                              BORDER_INNER_COLOR, BORDER_INNER_WIDTH);

}

void Game::drawMulti() const {
    auto &players = client->players;
    auto &player_list = client->player_list;

    for (int p = 0; p < player_list.size(); p++) {
        const int fd = player_list[p];
        auto &[player_name, player_board, player_alive] = players[fd];
        al_draw_filled_rectangle(MULTI_X[p], MULTI_Y[p],
                                 MULTI_X[p] + MULTI_WIDTH, MULTI_Y[p] + MULTI_HEIGHT,
                                 MULTI_BG_COLOR);

        for (int i = 0; i < TILE_COUNT_V; i++) {
            for (int j = 0; j < TILE_COUNT_H; j++) {
                if (player_board[i][j] == Tile::NONE)
                    continue;
                al_draw_tinted_scaled_bitmap(TetrisController::tetrimino_textures[int(player_board[i][j])],
                                             al_map_rgba_f(TETROMINO_BOARD_ALPHA, TETROMINO_BOARD_ALPHA, TETROMINO_BOARD_ALPHA, TETROMINO_BOARD_ALPHA),
                                             0, 0, TETROMINO_BLOCK_TEXTURE_SIZE, TETROMINO_BLOCK_TEXTURE_SIZE,
                                             MULTI_X[p] + MULTI_TILE_SIZE * j, MULTI_Y[p] + MULTI_TILE_SIZE * (TILE_COUNT_V - i - 1),
                                             MULTI_TILE_SIZE, MULTI_TILE_SIZE,
                                             0);
            }
        }

        if (!player_alive) {
            al_draw_filled_rectangle(MULTI_X[p], MULTI_Y[p],
                                     MULTI_X[p] + MULTI_WIDTH, MULTI_Y[p] + MULTI_HEIGHT,
                                     al_map_rgba(20, 20, 20, 150));
        }
    }
}

void Game::drawTexts() const {
    if (status == GameStatus::PENDING) {
        if (gameType == GameType::MULTI_CLIENT) {
            al_draw_multiline_text(Window::AirStrike40, TEXT_COLOR,
                                   GAMEPLAY_X + GAMEPLAY_WIDTH/2.0, GAMEPLAY_Y + GAMEPLAY_HEIGHT/2.0,
                                   GAMEPLAY_WIDTH, 40,
                                   ALLEGRO_ALIGN_CENTER, "Waiting host to start game");
        } else {
            al_draw_multiline_text(Window::AirStrike40, TEXT_COLOR,
                                   GAMEPLAY_X + GAMEPLAY_WIDTH/2.0, GAMEPLAY_Y + GAMEPLAY_HEIGHT/2.0,
                                   GAMEPLAY_WIDTH, 40,
                                   ALLEGRO_ALIGN_CENTER, "Press ENTER to\nstart game");
        }
    }

}

void Game::ReceiveAttack(int lines) {
    if (status == GameStatus::PLAYING)
        tc->ReceiveAttack(lines);
}

