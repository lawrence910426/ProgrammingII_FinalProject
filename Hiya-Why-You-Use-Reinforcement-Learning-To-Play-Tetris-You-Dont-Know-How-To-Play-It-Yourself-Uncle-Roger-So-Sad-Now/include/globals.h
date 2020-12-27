#pragma once

#include <utility>

#define FPS 60
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

enum class GameStatus {
  MENU, CONNECTING, LOADING, PLAYING, ENDING, EXIT
};

extern GameStatus gameStatus;

enum class GameType {
    SINGLE, MULTI_HOST, MULTI_CLIENT, EXIT
};

enum class GameResult {
    EXIT, WIN, LOSE
};

struct Position {
    int x;
    int y;
};