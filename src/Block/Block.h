#pragma once

#include "../Game/Game.h"

class Game;

class Block {
public:
    friend class Game;

private:
    Block() {
        ct[0] = (Point{0, 0});
        ct[1] = (Point{1, 0});
        ct[2] = (Point{2, 0});
        ct[3] = (Point{0, 1});
        ct[4] = (Point{0, 2});
        ct[5] = (Point{0, RECT_Y - 1});
        ct[6] = (Point{0, RECT_Y - 2});
        ct[7] = (Point{0, RECT_Y - 3});
        ct[8] = (Point{1, RECT_Y - 1});
        ct[9] = (Point{2, RECT_Y - 1});
        ct[10] = (Point{RECT_X - 1, 0});
        ct[11] = (Point{RECT_X - 2, 0});
        ct[12] = (Point{RECT_X - 3, 0});
        ct[13] = (Point{RECT_X - 1, 1});
        ct[14] = (Point{RECT_X - 1, 2});
        ct[15] = (Point{RECT_X - 1, RECT_Y - 1});
        ct[16] = (Point{RECT_X - 1, RECT_Y - 2});
        ct[17] = (Point{RECT_X - 1, RECT_Y - 3});
        ct[18] = (Point{RECT_X - 2, RECT_Y - 1});
        ct[19] = (Point{RECT_X - 3, RECT_Y - 1});
    };
    std::array<Point, 20> ct;
};