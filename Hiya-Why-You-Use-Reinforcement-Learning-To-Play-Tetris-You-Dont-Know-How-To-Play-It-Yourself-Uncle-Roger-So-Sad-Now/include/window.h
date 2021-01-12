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

    void Start();

    static ALLEGRO_FONT *TechFont35;
    static ALLEGRO_FONT *AirStrike40;
    static ALLEGRO_FONT *AirStrike30;

 private:
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *tick;
};