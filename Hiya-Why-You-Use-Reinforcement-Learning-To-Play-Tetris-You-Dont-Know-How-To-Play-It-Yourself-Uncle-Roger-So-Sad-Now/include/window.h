#pragma once

#include "globals.h"
#include "log.h"

#include "menu.h"
#include "game.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class Window {
 public:
    Window();
    ~Window();

    static void Start();

 private:
    ALLEGRO_DISPLAY *display;
};