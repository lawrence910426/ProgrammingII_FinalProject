#pragma once

#include <utility>

enum class GameStatus {
  MENU, CONNECTING, LOADING, PLAYING, ENDING, EXIT
};

GameStatus gameStatus;


enum class GameType {
    SINGLE, MULTI_HOST, MULTI_CLIENT, EXIT
};

struct Position {
    int x;
    int y;
};