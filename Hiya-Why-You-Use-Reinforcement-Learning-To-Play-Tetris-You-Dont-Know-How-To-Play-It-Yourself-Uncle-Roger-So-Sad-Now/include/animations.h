#pragma once

#include <allegro5/allegro5.h>

class ClearLineAnimation {
 public:
    ClearLineAnimation(int posy);

    bool NextFrame();

 private:
    int frame = 0;
    int y;

    static ALLEGRO_BITMAP *texture;
    static bool texture_loaded;
};