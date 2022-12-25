#pragma once

#include "../Game/Game.h"

class Game;

class BigFruit {
public:
    friend class Game;

private:
    BigFruit() {
        lastSnake_ = 'n';
        lastScore_ = 5;
        isExisting_ = false;
        for (size_t i = 0; i < 4; i++) cr[i] = Point{0, 0};
    }

    char lastSnake_;
    size_t lastScore_;
    bool isExisting_;
    Point cr[4];
};