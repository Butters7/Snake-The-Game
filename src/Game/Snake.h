#include <conio.h>
#include <random>

class Game;

static const int SPEED = 20;
static const int FRAME_RATE = 1000 / SPEED;
static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 640;
static const int GRID_WIDTH = 20;
static const int GRID_HEIGHT = 20;
static const int RECT_X = SCREEN_WIDTH / GRID_WIDTH;
static const int RECT_Y = SCREEN_HEIGHT / GRID_HEIGHT;
static const int COUNT_OF_GRID = RECT_X * RECT_Y;

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

    //Constructor with srand() providing a working random
    Snake();

    //Default settings of a snake
    void defaultSnake();

    //Spawn fruit and check did it spawn in a snake
    void spawnFruit();

    //Indicate direction
    void changeDirection();

    //Here we need to create 4 variables to remember previous location of all snake's elements
    void moving();

    //If snake went off the map move her to the opposite side
    void endProcess();

    //If snake ate the fruit add score and her length
    bool eatingProcess();

    //If snake hit the tail restart the game
    bool plungingCheck();

    int moveX_;
    int moveY_;
    int score_;
    int length_;
    int tailX_[COUNT_OF_GRID];
    int tailY_[COUNT_OF_GRID];
    int blockX_[5];
    int blockY_[5];
    int fruitX_;
    int fruitY_;
    Direction dir_;
};