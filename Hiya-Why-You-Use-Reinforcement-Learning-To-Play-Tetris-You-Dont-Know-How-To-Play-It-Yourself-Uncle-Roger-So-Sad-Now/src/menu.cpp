#include "menu.h"
#include "log.h"
#include "window.h"



using namespace Constants;

// TODO Menu

bool Menu::texture_loaded = false;
ALLEGRO_BITMAP *Menu::background_img;
ALLEGRO_BITMAP *Menu::hiya_img;

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

Menu::Menu(ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *tick) {
    eventQueue = al_create_event_queue();
    if (!eventQueue)
        FATAL("Failed to create event queue!");

    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(tick));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
//
//    al_clear_to_color(BACKGROUND_COLOR);
//    al_draw_multiline_text(Window::AirStrike40, al_map_rgb(200, 200, 200),
//                 GAMEPLAY_X + GAMEPLAY_WIDTH/2.0, GAMEPLAY_Y + GAMEPLAY_HEIGHT/2.0,
//                           WINDOW_WIDTH, TILE_SIZE,
//                 ALLEGRO_ALIGN_CENTER, "Press ENTER to play single\nPress SPACE to host a new game\nPress RSHIFT to join a game");
//    al_flip_display();

    if (!texture_loaded) {
        background_img = al_load_bitmap("../assets/menu-bg.jpg");
        hiya_img = al_load_bitmap("../assets/haiya.png");

        texture_loaded = true;
    }

    bzero(Window::name, sizeof(Window::name));
    bzero(Window::host, sizeof(Window::host));

}

Menu::~Menu() {
    al_destroy_event_queue(eventQueue);
}

GameType Menu::Start() {
    ALLEGRO_EVENT event;
    for (;;) {
        al_wait_for_event(eventQueue, &event);

        static bool caps = false;
        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                return GameType::EXIT;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (handleKeyPress(event.keyboard.keycode))
                    return selection;

            case ALLEGRO_EVENT_TIMER:
                draw();
        }
    }
}

void Menu::draw() {
    al_draw_scaled_bitmap(background_img,
                          0, 0, al_get_bitmap_width(background_img), al_get_bitmap_height(background_img),
                          0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
                          0);
    if (state != MenuState::TITLE)
        al_draw_filled_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_map_rgba_f(0, 0, 0, 0.4));

    if (state == MenuState::TITLE) {
        static double last_draw = al_get_time() - 0.6;
        if (al_get_time() - last_draw >= 0.6) {
            al_draw_text(Window::AirStrike55, al_map_rgb(255, 255, 255),
                         WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 4.0 * 3,
                         ALLEGRO_ALIGN_CENTER, "Press ENTER to START");

            if (al_get_time() - last_draw >= 1.3)
                last_draw = al_get_time();
        }
        const int WIDTH = 500;
        const int HEIGHT = 300;
        al_draw_scaled_bitmap(hiya_img, 0, 0, al_get_bitmap_width(hiya_img), al_get_bitmap_height(hiya_img),
                              (WINDOW_WIDTH - WIDTH)/2.0, WINDOW_HEIGHT / 10.0, WIDTH, HEIGHT, 0);
        al_draw_multiline_text(Window::AirStrike80, al_map_rgb(185, 3, 30),
                     WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0,
                               WINDOW_WIDTH /6.0 * 5, 50.0,
                     ALLEGRO_ALIGN_CENTER, "why you use reinforcement learning to play tetris\nuncle roger so sad now");
    } else if (state == MenuState::SELECT_GAME) {
        al_draw_text(Window::AirStrike80, al_map_rgb(255, 255, 255),
                     WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 6.0,
                     ALLEGRO_ALIGN_CENTER, "SELECT GAME");

        al_draw_filled_rounded_rectangle(WINDOW_WIDTH / 10.0, WINDOW_HEIGHT / 10.0 * 4,
                                         WINDOW_WIDTH / 10.0 * 9, WINDOW_HEIGHT / 10.0 * 9,
                                         50.0, 50.0,
                                         al_map_rgba_f(0, 0, 0, 0.2));

        char const *info_txt = selection == GameType::SINGLE? "Play singleplayer":
                                selection == GameType::MULTI_CLIENT? "Connect to another game":
                                selection == GameType::MULTI_HOST? "Host a new game": "";
        al_draw_text(Window::AirStrike55, al_map_rgb(255, 255, 255),
                     WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 10.0 * 5,
                     ALLEGRO_ALIGN_CENTER, info_txt);

        const ALLEGRO_COLOR sel_clr = al_map_rgb(209, 235, 64);
        const ALLEGRO_COLOR dis_clr = al_map_rgb(85, 168, 170);
        const ALLEGRO_FONT *sel_font = Window::AirStrike70;
        const ALLEGRO_FONT *dis_font = Window::AirStrike55;

        al_draw_text(selection == GameType::SINGLE? sel_font: dis_font,
                     selection == GameType::SINGLE? sel_clr: dis_clr,
                     WINDOW_WIDTH / 4.0 * 1, WINDOW_HEIGHT / 4.0 * 3,
                     ALLEGRO_ALIGN_CENTER, "SINGLE");
        al_draw_text(selection == GameType::MULTI_CLIENT? sel_font: dis_font,
                     selection == GameType::MULTI_CLIENT? sel_clr: dis_clr,
                     WINDOW_WIDTH / 4.0 * 2, WINDOW_HEIGHT / 4.0 * 3,
                     ALLEGRO_ALIGN_CENTER, "CLIENT");
        al_draw_text(selection == GameType::MULTI_HOST? sel_font: dis_font,
                     selection == GameType::MULTI_HOST? sel_clr: dis_clr,
                     WINDOW_WIDTH / 4.0 * 3, WINDOW_HEIGHT / 4.0 * 3,
                     ALLEGRO_ALIGN_CENTER, "HOST");
    } else if (state == MenuState::ENTER_NAME) {
        al_draw_text(Window::AirStrike80, al_map_rgb(255, 255, 255),
                     WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 6.0,
                     ALLEGRO_ALIGN_CENTER, "ENTER NAME");

        al_draw_text(Window::AirStrike55, al_map_rgb(255, 255, 255),
                     WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 10.0 * 5,
                     ALLEGRO_ALIGN_CENTER, "Enter your name");

        al_draw_filled_rounded_rectangle(WINDOW_WIDTH / 10.0, WINDOW_HEIGHT / 10.0 * 4,
                                         WINDOW_WIDTH / 10.0 * 9, WINDOW_HEIGHT / 10.0 * 9,
                                         50.0, 50.0,
                                         al_map_rgba_f(0, 0, 0, 0.2));

        al_draw_filled_rounded_rectangle(WINDOW_WIDTH / 10.0 * 1.3, WINDOW_HEIGHT / 10.0 * 6.8,
                                         WINDOW_WIDTH / 10.0 * 8.7, WINDOW_HEIGHT / 10.0 * 8,
                                         10.0, 10.0,
                                         al_map_rgba_f(0.6, 0.6, 0.6, 0.6));

        al_draw_text(Window::AirStrike70,
                     al_map_rgb(209, 235, 64),
                     WINDOW_WIDTH / 4.0 * 2, WINDOW_HEIGHT / 10.0 * 7.05,
                     ALLEGRO_ALIGN_CENTER, Window::name);
    } else if (state == MenuState::ENTER_HOST) {
        al_draw_text(Window::AirStrike80, al_map_rgb(255, 255, 255),
                     WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 6.0,
                     ALLEGRO_ALIGN_CENTER, "ENTER HOST");

        al_draw_text(Window::AirStrike55, al_map_rgb(255, 255, 255),
                     WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 10.0 * 5,
                     ALLEGRO_ALIGN_CENTER, "Enter the hostname/IP of the host");

        al_draw_filled_rounded_rectangle(WINDOW_WIDTH / 10.0, WINDOW_HEIGHT / 10.0 * 4,
                                         WINDOW_WIDTH / 10.0 * 9, WINDOW_HEIGHT / 10.0 * 9,
                                         50.0, 50.0,
                                         al_map_rgba_f(0, 0, 0, 0.2));

        al_draw_filled_rounded_rectangle(WINDOW_WIDTH / 10.0 * 1.3, WINDOW_HEIGHT / 10.0 * 6.8,
                                         WINDOW_WIDTH / 10.0 * 8.7, WINDOW_HEIGHT / 10.0 * 8,
                                         10.0, 10.0,
                                         al_map_rgba_f(0.6, 0.6, 0.6, 0.6));

        al_draw_text(Window::AirStrike70,
                     al_map_rgb(209, 235, 64),
                     WINDOW_WIDTH / 4.0 * 2, WINDOW_HEIGHT / 10.0 * 7.05,
                     ALLEGRO_ALIGN_CENTER, Window::host);
    }

    al_flip_display();
}

bool Menu::handleKeyPress(int key) {
    if (state == MenuState::TITLE) {
        if (key == ALLEGRO_KEY_ENTER) {
            al_play_sample(Window::se_enter, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
            state = MenuState::SELECT_GAME;
        }
    } else if (state == MenuState::SELECT_GAME) {
        if (key == ALLEGRO_KEY_ENTER) {
            if (selection == GameType::SINGLE) {
                al_play_sample(Window::se_enter, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
                return true;
            } else {
                al_play_sample(Window::se_select, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
                state = MenuState::ENTER_NAME;
            }
        } else if (key == ALLEGRO_KEY_LEFT) {
            al_play_sample(Window::se_switch, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
            if (selection > GameType::SINGLE)
                selection = GameType(int(selection) - 1);
        } else if (key == ALLEGRO_KEY_RIGHT) {
            al_play_sample(Window::se_switch, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
            if (selection < GameType::MULTI_HOST)
                selection = GameType(int(selection) + 1);
        }
    } else if (state == MenuState::ENTER_NAME || state == MenuState::ENTER_HOST) {
        char *input = state == MenuState::ENTER_NAME? Window::name: Window::host;
        int &siz = state == MenuState::ENTER_NAME? namep: hostp;

        if (key == ALLEGRO_KEY_ENTER) {
            if (selection == GameType::MULTI_HOST || state == MenuState::ENTER_HOST) {
                al_play_sample(Window::se_enter, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
                return true;
            } else {
                al_play_sample(Window::se_select, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
                state = MenuState::ENTER_HOST;
            }
        } else if (key == ALLEGRO_KEY_BACKSPACE) {
            if (siz > 0)
                siz--;
            al_play_sample(Window::se_move, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
        } else if (key >= ALLEGRO_KEY_A && key <= ALLEGRO_KEY_Z) {
            input[siz++] = key - ALLEGRO_KEY_A + 'A';
            al_play_sample(Window::se_move, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
        } else if (key >= ALLEGRO_KEY_0 && key <= ALLEGRO_KEY_9) {
            input[siz++] = key - ALLEGRO_KEY_0 + '0';
            al_play_sample(Window::se_move, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
        } else if (key >= ALLEGRO_KEY_PAD_0 && key <= ALLEGRO_KEY_PAD_9) {
            input[siz++] = key - ALLEGRO_KEY_PAD_0 + '0';
            al_play_sample(Window::se_move, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
        } else if (key == ALLEGRO_KEY_SPACE) {
            input[siz++] = ' ';
            al_play_sample(Window::se_move, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
        } else if (key == 73 || key == 90) {
            input[siz++] = '.';
            al_play_sample(Window::se_move, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
        }

        input[siz] = '\0';
    }
    return false;
}
