#pragma once

#include "../Game/Game.h"

class Game;

class Block {
public:
    friend class Game;

private:
    Block() {
        ct.push_back(Point{0, 0});
        ct.push_back(Point{1, 0});
        ct.push_back(Point{2, 0});
        ct.push_back(Point{0, 1});
        ct.push_back(Point{0, 2});
        ct.push_back(Point{0, RECT_Y - 1});
        ct.push_back(Point{0, RECT_Y - 2});
        ct.push_back(Point{0, RECT_Y - 3});
        ct.push_back(Point{1, RECT_Y - 1});
        ct.push_back(Point{2, RECT_Y - 1});
        ct.push_back(Point{RECT_X - 1, 0});
        ct.push_back(Point{RECT_X - 2, 0});
        ct.push_back(Point{RECT_X - 3, 0});
        ct.push_back(Point{RECT_X - 1, 1});
        ct.push_back(Point{RECT_X - 1, 2});
        ct.push_back(Point{RECT_X - 1, RECT_Y - 1});
        ct.push_back(Point{RECT_X - 1, RECT_Y - 2});
        ct.push_back(Point{RECT_X - 1, RECT_Y - 3});
        ct.push_back(Point{RECT_X - 2, RECT_Y - 1});
        ct.push_back(Point{RECT_X - 3, RECT_Y - 1});
    };
    std::vector<Point> ct;
};