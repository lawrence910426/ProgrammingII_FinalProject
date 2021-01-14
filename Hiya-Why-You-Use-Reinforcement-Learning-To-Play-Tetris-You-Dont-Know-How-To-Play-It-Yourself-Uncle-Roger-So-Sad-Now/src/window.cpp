#include "window.h"

using namespace Constants;

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

ALLEGRO_FONT *Window::TechFont35;
ALLEGRO_FONT *Window::AirStrike40;
ALLEGRO_FONT *Window::AirStrike30;
ALLEGRO_FONT *Window::AirStrike55;
ALLEGRO_FONT *Window::AirStrike70;
ALLEGRO_FONT *Window::AirStrike80;

char Window::name[50];
char Window::host[50];

Window::Window() {
    if (!al_init())
        FATAL("Allegro init failed!");
    if (!al_install_keyboard())
        FATAL("Keyboard init failed!");

    tick = al_create_timer(1.0 / FPS);
    if(!tick)
        FATAL("couldn't initialize timer\n");

    display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!display)
        FATAL("Display init failed!")

    if(!al_init_image_addon())
        FATAL("Image addon init failed!");

    if (!al_init_font_addon())
        FATAL("Font addon init failed!")

    if (!al_init_ttf_addon())
        FATAL("Font addon init failed!")

    init_colors();

    TechFont35 = al_load_ttf_font("../assets/techno_hideo.ttf", 35, 0);
    AirStrike40 = al_load_ttf_font("../assets/airstrike.ttf", 40, 0);
    AirStrike30 = al_load_ttf_font("../assets/airstrike.ttf", 30, 0);
    AirStrike55 = al_load_ttf_font("../assets/airstrike.ttf", 55, 0);
    AirStrike70 = al_load_ttf_font("../assets/airstrike.ttf", 70, 0);
    AirStrike80 = al_load_ttf_font("../assets/airstrike.ttf", 80, 0);

}

Window::~Window() {
    al_uninstall_keyboard();
    al_destroy_display(display);
    al_destroy_timer(tick);
}

void Window::Start() {
    al_start_timer(tick);
    for (;;) {
        auto *menu = new Menu(display, tick);
        GameType selection = menu->Start();
        delete menu;

        INFO("Game selected: " << int(selection));
        if (selection == GameType::EXIT)
            return;

        auto *game = new Game(selection, display, tick, name, host);
        GameResult result = game->Start();
        delete game;

        if (result == GameResult::EXIT)
            return;
    }
}
