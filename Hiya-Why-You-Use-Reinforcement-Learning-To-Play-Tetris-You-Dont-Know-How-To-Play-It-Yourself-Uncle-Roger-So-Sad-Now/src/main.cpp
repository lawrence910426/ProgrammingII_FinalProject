#include "game.h"

int main(int argc, char *argv[]) {
    auto *Game= new TetrisGame();

//    Game->game_play();

    delete Game;
    return 0;
}
