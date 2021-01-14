#pragma once

#include <string>
#include <allegro5/allegro.h>

#include "globals.h"



class Menu {
 public:
    Menu(ALLEGRO_DISPLAY*, ALLEGRO_TIMER*);
    ~Menu();

    GameType Start();

    int namep = 0;
    int hostp = 0;

    static ALLEGRO_BITMAP *background_img;
    static ALLEGRO_BITMAP *hiya_img;
 private:
    static bool texture_loaded;
    ALLEGRO_EVENT_QUEUE *eventQueue;

    MenuState state = MenuState::TITLE;

    GameType selection = GameType::SINGLE;

    bool handleKeyPress(int key);

    void draw();



};
