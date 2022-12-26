#pragma once

#include "../Consts/Consts.h"
#include <array>
#include <cmath>

class Game;

enum Direction {
    STOP_DIRECTION,
    UP_DIRECTION,
    DOWN_DIRECTION,
    LEFT_DIRECTION,
    RIGHT_DIRECTION,
};

class Snake {
public:
    friend class Game;

private:

    //Declare private constructor
    Snake() {
        length_ = 3;
        dir_ = STOP_DIRECTION;
        tail.fill(Point{0, 0});
        move = Point{0, 0};
    };

    //Default settings of a snake
    void defaultSnake(const int &number);

    //Indicate direction
    void changeDirection();

    //Here we need to create 4 variables to remember previous location of all snake's elements
    void moving();

    //If snake went off the map move her to the opposite side
    void endProcess();

    //If snake ate the fruit add score and her length
    void snakeGrowth();

    //If snake hit the tail restart the game
    bool plungingTailCheck();

private:
    int length_;
    Point move;
    std::array<Point, WIN_SCORE + MIN_LENGTH + 1> tail;
    Direction dir_;
};