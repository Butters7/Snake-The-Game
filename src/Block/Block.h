#pragma once

#include "../Game/Game.h"

class Game;

class Block {
public:
    friend class Game;

private:
    Block() {};
    int x_[5];
    int y_[5];
};