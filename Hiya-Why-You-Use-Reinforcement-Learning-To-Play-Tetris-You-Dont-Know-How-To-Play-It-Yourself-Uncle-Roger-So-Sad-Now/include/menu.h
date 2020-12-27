#pragma once

#include <allegro5/allegro.h>

#include <string>

#include "globals.h"

class Menu {
 public:
    Menu(ALLEGRO_DISPLAY*, ALLEGRO_TIMER*);
    ~Menu();

    GameType Start();

 private:
    static bool init;
    ALLEGRO_EVENT_QUEUE *eventQueue;

    void drawBackground();



};

class MenuButton {
 public:
    MenuButton(std::string title, ALLEGRO_COLOR color, Position pos);
    void Draw();
    void Select();
    void Unselect();

 private:
    std::string title;
    ALLEGRO_COLOR color;
    Position pos;
    bool selected = false;
};