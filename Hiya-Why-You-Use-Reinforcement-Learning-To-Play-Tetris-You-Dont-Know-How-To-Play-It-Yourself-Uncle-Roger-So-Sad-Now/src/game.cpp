#include "game.h"

using namespace Constants;

#include <map>

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
    if (type == GameType::SINGLE) {

    }

    tc = new TetrisController();
}

Game::~Game() {
    al_destroy_event_queue(eventQueue);
    delete tc;
}

GameResult Game::Start() {
    ALLEGRO_EVENT event;
    al_start_timer(fall);
    al_start_timer(das);

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
                    for (auto& [keycode, val]: das_map) {
                        auto &[holding, last_hold_time] = val;
                        if (holding) {
                            if (al_get_time() - last_hold_time >= DAS_HOLD_SECONDS) {
                                handleKeyPress(keycode);
                            }
                        }
                    }
                } else { // tick
                    updateScreen();
                }
                break;
        }
    }
}

void Game::handleKeyPress(int keycode) {
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
}

void Game::updateScreen() {
    drawBackground();
    tc->Draw();

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
    al_draw_rounded_rectangle(GAMEPLAY_X - BORDER_OUTER_WIDTH/2.0, GAMEPLAY_Y - BORDER_OUTER_WIDTH/2.0,
                              GAMEPLAY_X + GAMEPLAY_WIDTH + BORDER_OUTER_WIDTH/2.0, GAMEPLAY_Y + GAMEPLAY_HEIGHT + BORDER_OUTER_WIDTH/2.0,
                              BORDER_OUTER_WIDTH / 2.0, BORDER_OUTER_WIDTH / 2.0,
                              BORDER_OUTER_COLOR, BORDER_OUTER_WIDTH);
    al_draw_rounded_rectangle(GAMEPLAY_X - BORDER_INNER_WIDTH/2.0, GAMEPLAY_Y - BORDER_INNER_WIDTH/2.0,
                              GAMEPLAY_X + GAMEPLAY_WIDTH + BORDER_INNER_WIDTH/2.0, GAMEPLAY_Y + GAMEPLAY_HEIGHT + BORDER_INNER_WIDTH/2.0,
                              BORDER_INNER_WIDTH / 2.0, BORDER_INNER_WIDTH / 2.0,
                              BORDER_INNER_COLOR, BORDER_INNER_WIDTH);

    /// Hold Area
    // Background
    al_draw_filled_rectangle(HOLDAREA_X, HOLDAREA_Y,
                             HOLDAREA_X + HOLDAREA_SIZE, HOLDAREA_Y + HOLDAREA_SIZE,
                             GAMEPLAY_BG_COLOR);
    // Border
    al_draw_rounded_rectangle(HOLDAREA_X - BORDER_OUTER_WIDTH/2.0, HOLDAREA_Y - BORDER_OUTER_WIDTH/2.0,
                              HOLDAREA_X + HOLDAREA_SIZE + BORDER_OUTER_WIDTH/2.0, HOLDAREA_Y + HOLDAREA_SIZE + BORDER_OUTER_WIDTH/2.0,
                              BORDER_OUTER_WIDTH / 2.0, BORDER_OUTER_WIDTH / 2.0,
                              BORDER_OUTER_COLOR, BORDER_OUTER_WIDTH);
    al_draw_rounded_rectangle(HOLDAREA_X - BORDER_INNER_WIDTH/2.0, HOLDAREA_Y - BORDER_INNER_WIDTH/2.0,
                              HOLDAREA_X + HOLDAREA_SIZE + BORDER_INNER_WIDTH/2.0, HOLDAREA_Y + HOLDAREA_SIZE + BORDER_INNER_WIDTH/2.0,
                              BORDER_INNER_WIDTH / 2.0, BORDER_INNER_WIDTH / 2.0,
                              BORDER_INNER_COLOR, BORDER_INNER_WIDTH);

    /// Garbage buffer
    // Background
    al_draw_filled_rectangle(GARBAGE_BUFFER_X, GARBAGE_BUFFER_Y,
                             GARBAGE_BUFFER_X + GARBAGE_BUFFER_WIDTH, GARBAGE_BUFFER_Y + GARBAGE_BUFFER_HEIGHT,
                             GAMEPLAY_BG_COLOR);
    // Border
    al_draw_rounded_rectangle(GARBAGE_BUFFER_X - BORDER_OUTER_WIDTH/2.0, GARBAGE_BUFFER_Y - BORDER_OUTER_WIDTH/2.0,
                              GARBAGE_BUFFER_X + GARBAGE_BUFFER_WIDTH + BORDER_OUTER_WIDTH/2.0, GARBAGE_BUFFER_Y + GARBAGE_BUFFER_HEIGHT + BORDER_OUTER_WIDTH/2.0,
                              BORDER_OUTER_WIDTH / 2.0, BORDER_OUTER_WIDTH / 2.0,
                              BORDER_OUTER_COLOR, BORDER_OUTER_WIDTH);
    al_draw_rounded_rectangle(GARBAGE_BUFFER_X - BORDER_INNER_WIDTH/2.0, GARBAGE_BUFFER_Y - BORDER_INNER_WIDTH/2.0,
                              GARBAGE_BUFFER_X + GARBAGE_BUFFER_WIDTH + BORDER_INNER_WIDTH/2.0, GARBAGE_BUFFER_Y + GARBAGE_BUFFER_HEIGHT + BORDER_INNER_WIDTH/2.0,
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
    al_draw_rounded_rectangle(PREVIEW_AREA_X - BORDER_OUTER_WIDTH/2.0, PREVIEW_AREA_Y - BORDER_OUTER_WIDTH/2.0,
                              PREVIEW_AREA_X + PREVIEW_AREA_WIDTH + BORDER_OUTER_WIDTH/2.0, PREVIEW_AREA_Y + PREVIEW_AREA_HEIGHT + BORDER_OUTER_WIDTH/2.0,
                              BORDER_OUTER_WIDTH / 2.0, BORDER_OUTER_WIDTH / 2.0,
                              BORDER_OUTER_COLOR, BORDER_OUTER_WIDTH);
    al_draw_rounded_rectangle(PREVIEW_AREA_X - BORDER_INNER_WIDTH/2.0, PREVIEW_AREA_Y - BORDER_INNER_WIDTH/2.0,
                              PREVIEW_AREA_X + PREVIEW_AREA_WIDTH + BORDER_INNER_WIDTH/2.0, PREVIEW_AREA_Y + PREVIEW_AREA_HEIGHT + BORDER_INNER_WIDTH/2.0,
                              BORDER_INNER_WIDTH / 2.0, BORDER_INNER_WIDTH / 2.0,
                              BORDER_INNER_COLOR, BORDER_INNER_WIDTH);

}


