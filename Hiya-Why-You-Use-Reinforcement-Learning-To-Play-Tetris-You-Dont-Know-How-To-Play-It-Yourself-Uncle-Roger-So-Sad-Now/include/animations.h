#pragma once

#include <allegro5/allegro5.h>
#include <list>

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

class StarEffect {
 public:
    StarEffect(int x, int y);

    bool NextFrame();

 private:
    int frame = 0;
    int sx, sy;

    double vx, vy;

    ALLEGRO_BITMAP *texture;

    static ALLEGRO_BITMAP *textures[3];
    static bool texture_loaded;
};

class SendLineAnimation {
 public:
    SendLineAnimation(int fromx, int fromy, int tox, int toy);

    bool NextFrame();

 private:
    int frame = 0;
    int sx, sy;
    double vx, vy;
    int end_frame;

    std::list<StarEffect *> stars;

    static ALLEGRO_BITMAP *texture;
    static bool texture_loaded;
};

