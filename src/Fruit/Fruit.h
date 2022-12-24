#pragma once

#include "../Game/Game.h"

class Game;

class Fruit {
public:
    friend class Game;

private:
    Fruit() {
        cr = Point{0, 0};
    };
    Point cr;
};