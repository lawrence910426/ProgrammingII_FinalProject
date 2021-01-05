#include "animations.h"

#include "globals.h"
using namespace Constants;
#include "log.h"

bool ClearLineAnimation::texture_loaded = false;
ALLEGRO_BITMAP *ClearLineAnimation::texture;

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
