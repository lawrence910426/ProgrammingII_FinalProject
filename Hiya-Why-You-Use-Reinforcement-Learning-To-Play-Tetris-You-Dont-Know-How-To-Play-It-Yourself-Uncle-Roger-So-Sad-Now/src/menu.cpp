#include "menu.h"
#include "log.h"

// TODO Menu

bool Menu::init = false;

Menu::Menu(ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *tick) {
    eventQueue = al_create_event_queue();
    if (!eventQueue)
        FATAL("Failed to create event queue!");

    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(tick));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
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

            case ALLEGRO_EVENT_TIMER:
                if (event.timer.count >= FPS * 10)
                    return GameType::EXIT;
        }
    }
}
