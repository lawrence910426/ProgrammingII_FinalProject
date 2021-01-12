#include "menu.h"
#include "log.h"
#include "window.h"



using namespace Constants;

// TODO Menu

bool Menu::init = false;

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

Menu::Menu(ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *tick) {
    eventQueue = al_create_event_queue();
    if (!eventQueue)
        FATAL("Failed to create event queue!");

    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(tick));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    al_clear_to_color(BACKGROUND_COLOR);
    al_draw_multiline_text(Window::AirStrike40, al_map_rgb(200, 200, 200),
                 GAMEPLAY_X + GAMEPLAY_WIDTH/2.0, GAMEPLAY_Y + GAMEPLAY_HEIGHT/2.0,
                           WINDOW_WIDTH, TILE_SIZE,
                 ALLEGRO_ALIGN_CENTER, "Press ENTER to play single\nPress SPACE to host a new game\nPress RSHIFT to join a game");
    al_flip_display();

}

Menu::~Menu() {
    al_destroy_event_queue(eventQueue);
}

GameType Menu::Start() {
    ALLEGRO_EVENT event;
    for (;;) {
        al_wait_for_event(eventQueue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                return GameType::EXIT;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    return GameType::SINGLE;
                else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
                    return GameType::MULTI_HOST;
                else if (event.keyboard.keycode == ALLEGRO_KEY_RSHIFT)
                    return GameType::MULTI_CLIENT;
                break;

            case ALLEGRO_EVENT_TIMER:
                if (event.timer.count >= FPS * 100)
                    return GameType::EXIT;
        }
    }
}
