#include "game.h"

TetrisGame::TetrisGame() {
    if (!al_init())
        FATAL("AL Init Failed :(");
    INFO("AL Init Success");
}

void TetrisGame::game_init() {

}
void TetrisGame::game_reset() {

}
