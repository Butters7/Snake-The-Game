#pragma once

#include "../Game/Game.h"

class Game;

class Fruit {
public:
    friend class Game;

private:
    Fruit() {};
    int x_;
    int y_;
};