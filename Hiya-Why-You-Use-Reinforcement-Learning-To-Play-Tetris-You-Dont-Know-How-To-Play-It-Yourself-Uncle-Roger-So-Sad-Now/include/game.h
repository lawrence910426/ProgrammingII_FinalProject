#pragma once

class Game {
 public:
    Game(GameType);
    ~Game();

    void Start();

 private:
    GameType gameType;
};