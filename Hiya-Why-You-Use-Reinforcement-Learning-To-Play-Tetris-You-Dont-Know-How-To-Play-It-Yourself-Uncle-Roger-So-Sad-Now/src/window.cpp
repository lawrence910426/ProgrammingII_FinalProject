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

ALLEGRO_SAMPLE *Window::menu_bgm;
ALLEGRO_SAMPLE *Window::gameplay_bgm;
ALLEGRO_SAMPLE *Window::se_single;
ALLEGRO_SAMPLE *Window::se_double;
ALLEGRO_SAMPLE *Window::se_triple;
ALLEGRO_SAMPLE *Window::se_tetris;
ALLEGRO_SAMPLE *Window::se_hold;
ALLEGRO_SAMPLE *Window::se_move;
ALLEGRO_SAMPLE *Window::se_harddrop;
ALLEGRO_SAMPLE *Window::se_attack;

ALLEGRO_SAMPLE_ID Window::menu_sampid;
ALLEGRO_SAMPLE_ID Window::gameplay_sampid;

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
        FATAL("TTF addon init failed!")

    if (!al_install_audio())
        FATAL("Audio init failed!")

    if (!al_init_acodec_addon())
        FATAL("Acodec init failed!")

    if (!al_reserve_samples(16))
        FATAL("Samples init failed!")


    init_colors();

    TechFont35 = al_load_ttf_font("../assets/techno_hideo.ttf", 35, 0);
    AirStrike40 = al_load_ttf_font("../assets/airstrike.ttf", 40, 0);
    AirStrike30 = al_load_ttf_font("../assets/airstrike.ttf", 30, 0);
    AirStrike55 = al_load_ttf_font("../assets/airstrike.ttf", 55, 0);
    AirStrike70 = al_load_ttf_font("../assets/airstrike.ttf", 70, 0);
    AirStrike80 = al_load_ttf_font("../assets/airstrike.ttf", 80, 0);

    menu_bgm = al_load_sample("../assets/menu-bgm.wav");
    gameplay_bgm = al_load_sample("../assets/gameplay-bgm.wav");
    se_single = al_load_sample("../assets/se_game_single.wav");
    se_double = al_load_sample("../assets/se_game_double.wav");
    se_triple = al_load_sample("../assets/se_game_triple.wav");
    se_tetris = al_load_sample("../assets/se_game_tetris.wav");
    se_hold = al_load_sample("../assets/se_game_hold.wav");
    se_move = al_load_sample("../assets/se_game_move.wav");
    se_harddrop = al_load_sample("../assets/se_game_harddrop.wav");
    se_attack = al_load_sample("../assets/se_game_attack1.wav");

}

Window::~Window() {
    al_uninstall_keyboard();
    al_destroy_display(display);
    al_destroy_timer(tick);
}

void Window::Start() {
    al_start_timer(tick);
    for (;;) {
        al_play_sample(menu_bgm, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &menu_sampid);
        auto *menu = new Menu(display, tick);
        GameType selection = menu->Start();
        delete menu;
        al_stop_sample(&menu_sampid);

        INFO("Game selected: " << int(selection));
        if (selection == GameType::EXIT)
            return;

        auto *game = new Game(selection, display, tick, name, host);
        GameResult result = game->Start();
        delete game;
        al_stop_sample(&gameplay_sampid);


        if (result == GameResult::EXIT)
            return;
    }
}
