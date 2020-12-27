#include "window.h"

GameStatus gameStatus;

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
        gameStatus = GameStatus::MENU;
        GameType selection = menu->Start();
        delete menu;

        INFO("Game selected");
        if (selection == GameType::EXIT)
            return;

        auto *game = new Game(selection, display, tick);
        GameResult result = game->Start();
        delete game;

        if (result == GameResult::EXIT)
            return;
    }
}
