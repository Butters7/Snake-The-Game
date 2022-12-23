#pragma once

#include "../Game/Game.h"

class Game;

class Block {
public:
    friend class Game;

private:
    Block() {};
    std::array<Point, 5> ct;
};