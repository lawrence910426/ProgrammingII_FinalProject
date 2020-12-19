#pragma once

#include "log.h"

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include <list>
#include <time.h>

#define GAME_INIT -1
#define GAME_SETTING 0
#define GAME_SELECT 1
#define GAME_BEGIN 2
#define GAME_CONTINUE 3
#define GAME_FAIL 4
#define GAME_TERMINATE 5
#define GAME_NEXT_LEVEL 6
#define GAME_EXIT 7


// clock rate
const float FPS = 60;

class TetrisGame {
    public:
        // constructor
        TetrisGame();
        // each process of scene
        void game_init();
        void game_reset();
        void game_play();
        void game_begin();

        int game_run();
        int game_update();

        void game_destroy();


        // process of updated event
        int process_event();
        // detect if mouse hovers over a rectangle
        bool mouse_hover(int, int, int, int);

        bool initial = true;

    private:
        ALLEGRO_BITMAP *background = nullptr;

        ALLEGRO_DISPLAY *display = nullptr;

        ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
        ALLEGRO_EVENT event;
        ALLEGRO_TIMER *timer = nullptr;
        ALLEGRO_TIMER *monster_pro = nullptr;

        ALLEGRO_SAMPLE *sample = nullptr;
        ALLEGRO_SAMPLE_INSTANCE *startSound = nullptr;
        ALLEGRO_SAMPLE_INSTANCE *clearSound = nullptr;
        ALLEGRO_SAMPLE_INSTANCE *failSound = nullptr;
        ALLEGRO_SAMPLE_INSTANCE *backgroundSound = nullptr;

};

