#include "window.h"


Window::Window() {
    if (!al_init())
        FATAL("Allegro init failed!");
    if (!al_install_keyboard())
        FATAL("Keyboard init failed!");

    display = al_create_display(800, 600);
    if (!display)
        FATAL("Display init failed!");
}

Window::~Window() {
    al_uninstall_keyboard();
    al_destroy_display(display);
    Menu().Free();
}

void Window::Start() {
    for (;;) {
        auto *menu = new Menu();
        gameStatus = GameStatus::MENU;
        GameType selection = menu->Start();
        delete menu;

        if (selection == GameType::EXIT)
            return;

        auto *game = new Game(selection);
        game->Start();
        delete game;
    }
}
