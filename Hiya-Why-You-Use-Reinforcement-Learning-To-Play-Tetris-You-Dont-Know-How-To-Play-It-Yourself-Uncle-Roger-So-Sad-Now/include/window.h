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
    static ALLEGRO_FONT *AirStrike55;
    static ALLEGRO_FONT *AirStrike70;
    static ALLEGRO_FONT *AirStrike80;

    static ALLEGRO_SAMPLE *menu_bgm;
    static ALLEGRO_SAMPLE *gameplay_bgm;
    static ALLEGRO_SAMPLE *se_single;
    static ALLEGRO_SAMPLE *se_double;
    static ALLEGRO_SAMPLE *se_triple;
    static ALLEGRO_SAMPLE *se_tetris;
    static ALLEGRO_SAMPLE *se_hold;
    static ALLEGRO_SAMPLE *se_move;
    static ALLEGRO_SAMPLE *se_harddrop;
    static ALLEGRO_SAMPLE *se_attack;

    static ALLEGRO_SAMPLE_ID menu_sampid;
    static ALLEGRO_SAMPLE_ID gameplay_sampid;



    static char name[50];
    static char host[50];


 private:
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *tick;
};