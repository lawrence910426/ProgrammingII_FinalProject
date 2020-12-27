#include "game.h"

Game::Game(GameType type, ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *tick) {
    eventQueue = al_create_event_queue();
    if (!eventQueue)
        FATAL("Failed to create event queue!");

    if (!al_init_primitives_addon())
        FATAL("Primitives addon init failed!");

    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(tick));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    gameType = type;
    if (type == GameType::SINGLE) {

    }
}

Game::~Game() {
    al_destroy_event_queue(eventQueue);
}


GameResult Game::Start() {
    ALLEGRO_EVENT event;
    for (;;) {
        al_wait_for_event(eventQueue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                return GameResult::EXIT;

            case ALLEGRO_EVENT_KEY_DOWN:
                break;

            case ALLEGRO_EVENT_TIMER:
                updateScreen();
                break;
        }
    }
}

void Game::updateScreen() {
    drawBackground();

    al_flip_display();
}

void Game::drawBackground() {
    /// Background
    al_draw_filled_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_map_rgb(210, 210, 210));

    /// Gameplay Area
    // Background
    al_draw_filled_rectangle(GAMEPLAY_X, GAMEPLAY_Y,
                             GAMEPLAY_X + GAMEPLAY_WIDTH, GAMEPLAY_Y + GAMEPLAY_HEIGHT,
                             al_map_rgb(57, 57, 57));
    // Girds
    for (int i = 0; i < TILE_COUNT_V; i++)
        al_draw_line(GAMEPLAY_X, GAMEPLAY_Y + i * TILE_SIZE,
                     GAMEPLAY_X + GAMEPLAY_WIDTH, GAMEPLAY_Y + i * TILE_SIZE,
                     al_map_rgb(47, 47, 47), GIRD_WIDTH);
    for (int i = 0; i < TILE_COUNT_H; i++)
        al_draw_line(GAMEPLAY_X + i * TILE_SIZE, GAMEPLAY_Y,
                     GAMEPLAY_X + i * TILE_SIZE, GAMEPLAY_Y + GAMEPLAY_HEIGHT,
                     al_map_rgb(47, 47, 47), GIRD_WIDTH);
    // Border
    al_draw_rounded_rectangle(GAMEPLAY_X - BORDER_OUTER_WIDTH/2.0, GAMEPLAY_Y - BORDER_OUTER_WIDTH/2.0,
                              GAMEPLAY_X + GAMEPLAY_WIDTH + BORDER_OUTER_WIDTH/2.0, GAMEPLAY_Y + GAMEPLAY_HEIGHT + BORDER_OUTER_WIDTH/2.0,
                              BORDER_OUTER_WIDTH / 2.0, BORDER_OUTER_WIDTH / 2.0,
                              al_map_rgb(58, 219, 209), BORDER_OUTER_WIDTH);
    al_draw_rounded_rectangle(GAMEPLAY_X - BORDER_INNER_WIDTH/2.0, GAMEPLAY_Y - BORDER_INNER_WIDTH/2.0,
                              GAMEPLAY_X + GAMEPLAY_WIDTH + BORDER_INNER_WIDTH/2.0, GAMEPLAY_Y + GAMEPLAY_HEIGHT + BORDER_INNER_WIDTH/2.0,
                              BORDER_INNER_WIDTH / 2.0, BORDER_INNER_WIDTH / 2.0,
                              al_map_rgb(36, 133, 122), BORDER_INNER_WIDTH);

}

