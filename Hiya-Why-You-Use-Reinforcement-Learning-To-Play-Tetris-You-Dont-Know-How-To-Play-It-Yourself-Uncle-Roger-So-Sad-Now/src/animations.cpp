#include "animations.h"

#include "globals.h"
using namespace Constants;
#include "log.h"

bool ClearLineAnimation::texture_loaded = false;
bool SendLineAnimation::texture_loaded = false;
bool StarEffect::texture_loaded = false;

ALLEGRO_BITMAP *ClearLineAnimation::texture;
ALLEGRO_BITMAP *SendLineAnimation::texture;
ALLEGRO_BITMAP *StarEffect::textures[3];

ClearLineAnimation::ClearLineAnimation(int posy): y(posy) {
    if (!texture_loaded) {
        texture = al_load_bitmap("../assets/clear-line-effect-2.png");
        texture_loaded = true;
    }
}

bool ClearLineAnimation::NextFrame() {
    if (frame <= CLEAR_LINE_ANIMATION_S1 * FPS) { // Stage 1
        const double percentage = frame / (CLEAR_LINE_ANIMATION_S1 * FPS);
        al_draw_tinted_scaled_bitmap(texture, al_map_rgba_f(CLEAR_LINE_ANIMATION_ALPHA, CLEAR_LINE_ANIMATION_ALPHA, CLEAR_LINE_ANIMATION_ALPHA, CLEAR_LINE_ANIMATION_ALPHA),
                                     0, 0, al_get_bitmap_width(texture), al_get_bitmap_height(texture),
                                  GAMEPLAY_X + GAMEPLAY_WIDTH/2.0 - (GAMEPLAY_WIDTH + 4 * TILE_SIZE) * percentage /2.0, GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - y - 1),
                                  (GAMEPLAY_WIDTH + 4 * TILE_SIZE) * percentage, TILE_SIZE,
                                  0);
    } else if (frame <= CLEAR_LINE_ANIMATION_S2 * FPS) { // Stage 2
        al_draw_tinted_scaled_bitmap(texture, al_map_rgba_f(CLEAR_LINE_ANIMATION_ALPHA, CLEAR_LINE_ANIMATION_ALPHA, CLEAR_LINE_ANIMATION_ALPHA, CLEAR_LINE_ANIMATION_ALPHA),
                                     0, 0, al_get_bitmap_width(texture), al_get_bitmap_height(texture),
                                  GAMEPLAY_X - 2 * TILE_SIZE, GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - y - 1),
                                  GAMEPLAY_WIDTH + 4 * TILE_SIZE, TILE_SIZE,
                                  0);
    } else if (frame <= CLEAR_LINE_ANIMATION_END * FPS) { // Stage 3
        const double percentage = (frame - CLEAR_LINE_ANIMATION_S2 * FPS) / (CLEAR_LINE_ANIMATION_END * FPS);
        const double alpha = CLEAR_LINE_ANIMATION_ALPHA * (1 - percentage);
        al_draw_tinted_scaled_bitmap(texture, al_map_rgba_f(alpha, alpha, alpha, alpha),
                                     0, 0, al_get_bitmap_width(texture), al_get_bitmap_height(texture),
                                     GAMEPLAY_X - 2 * TILE_SIZE, GAMEPLAY_Y + TILE_SIZE * (TILE_COUNT_V - y - 1),
                                     GAMEPLAY_WIDTH + 4 * TILE_SIZE, TILE_SIZE,
                                     0);
    } else { // End
        return true;
    }

    frame++;
    return false;
}

SendLineAnimation::SendLineAnimation(int fromx, int fromy, int tox, int toy) : sx(fromx), sy(fromy){
    if (!texture_loaded) {
        texture = al_load_bitmap("../assets/yellow-ball.png");
        texture_loaded = true;
    }

    INFO("Constructor called");
    const int dx = tox - fromx;
    const int dy = toy - fromy;
    const double d = sqrt(dx*dx + dy*dy);
    vx = dx * (SEND_LINE_ANIMATION_SPEED / d);
    vy = dy * (SEND_LINE_ANIMATION_SPEED / d);
    end_frame = d / SEND_LINE_ANIMATION_SPEED;
}

bool SendLineAnimation::NextFrame() {
    INFO("Next frame");
    if (frame < end_frame) {
        INFO("Draw animation");
        const int x = sx + vx * frame;
        const int y = sy + vy * frame;
        const int bitmap_w = al_get_bitmap_width(texture);
        const int bitmap_h = al_get_bitmap_height(texture);
        al_draw_tinted_scaled_bitmap(texture, al_map_rgba_f(0.8, 0.8, 0.8, 0.8),
                                     0, 0, bitmap_w, bitmap_h,
                                     x - TILE_SIZE, y - TILE_SIZE,
                                     2 * TILE_SIZE, 2 * TILE_SIZE,
                                     0);

        for (int i = 0; i < SEND_LINE_STAR_EFFECT_PER_FRAME; i++)
            stars.emplace_back(new StarEffect(x, y));
    }

    for (auto it = stars.begin(); it != stars.end(); ) {
        if ((*it)->NextFrame()) {
            delete *it;
            it = stars.erase(it);
        } else
            it++;
    }

    if (frame >= end_frame && stars.empty())
        return true;

    frame++;
    return false;
}


StarEffect::StarEffect(int x, int y): sx(x + randint(-20, 20)), sy(y + randint(-20, 20)) {
    if (!texture_loaded) {
        textures[0] = al_load_bitmap("../assets/yellow-ball.png");
        textures[1] = al_load_bitmap("../assets/star.png");
        textures[2] = al_load_bitmap("../assets/light-star.png");
        texture_loaded = true;
    }

    texture = textures[randint(0, 2)];
    vx = randint(-20, 20) / 10.0;
    vy = randint(-20, 20) / 10.0;
}

bool StarEffect::NextFrame() {
    if (frame < FPS * STAR_EFFECT_LAST_SEC) {
        const double alpha = 1.0 - double(frame)/(FPS * STAR_EFFECT_LAST_SEC);
        const int x = sx + vx * frame;
        const int y = sy + vy * frame;
        const int bitmap_w = al_get_bitmap_width(texture);
        const int bitmap_h = al_get_bitmap_height(texture);
        al_draw_tinted_scaled_bitmap(texture, al_map_rgba_f(alpha, alpha, alpha, alpha),
                                     0, 0, bitmap_w, bitmap_h,
                                     x - STAR_EFFECT_SIZE/2.0, y - STAR_EFFECT_SIZE/2.0,
                                     STAR_EFFECT_SIZE, STAR_EFFECT_SIZE,
                                     0);
    } else
        return true;

    frame++;
    return false;
}
