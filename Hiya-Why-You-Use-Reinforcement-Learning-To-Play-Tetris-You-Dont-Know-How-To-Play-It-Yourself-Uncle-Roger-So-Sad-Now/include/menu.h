#pragma once

#include <allegro5/allegro.h>

#include <string>

#include "globals.h"

class Menu {
 public:
    Menu();
    ~Menu();

    GameType Start();

    void Free();
 private:
    static bool init;

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