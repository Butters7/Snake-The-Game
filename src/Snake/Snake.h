#pragma once

#include "../Consts/Consts.h"
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
    Snake() {};

    //Default settings of a snake
    void defaultSnake(const int &X_, const int &Y_);

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

    int moveX_;
    int moveY_;
    int length_;
    int tailX_[COUNT_OF_GRID];
    int tailY_[COUNT_OF_GRID];
    Direction dir_;
};