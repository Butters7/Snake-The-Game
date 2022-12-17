#include <random>
#include <conio.h>
#include <SDL.h>
#include <thread>

namespace GAME {

    const int FRAME_RATE = 1000 / 60;
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 640;
    const int GRID_WIDTH = 32;
    const int GRID_HEIGHT = 32;
    const int RECT_X = SCREEN_WIDTH / GRID_WIDTH;
    const int RECT_Y = SCREEN_HEIGHT / GRID_HEIGHT;
    const int COUNT_OF_GRID = RECT_X * RECT_Y;

    enum Direction {
        UP_DIRECTION,
        DOWN_DIRECTION,
        LEFT_DIRECTION,
        RIGHT_DIRECTION,
    };

    class Snake {
    public:
        //Constructor with srand() providing a working random
        Snake();

        //Initial snake settings
        void start();

    private:

        //Default settings of a snake
        void defaultSnake();

        //Our game starting here
        void game();

        void draw();

        void initializeSDL();

        //Spawn fruit and check did it spawn in a snake
        void spawnFruit();

        //Update window title
        void upDateWindowTitle();

        //Check pressed key and change directory
        void clickHandler();

        //Indicate direction
        void changeDirection();

        //Here we need to create 4 variables to remember previous location of all snake's elements
        void moving();

        //If snake went off the map move her to the opposite side
        void endProcess();

        //If snake ate the fruit add score and her length
        void eatingProcess();

        //If snake hit the tail restart the game
        void plungingCheck();

    private:
        int moveX_;
        int moveY_;
        bool is_playing_;
        int score_;
        int length_;
        int tailX_[COUNT_OF_GRID];
        int tailY_[COUNT_OF_GRID];
        int fruitX_;
        int fruitY_;
        int fps_;
        Direction dir_;
        SDL_Window *window_ = nullptr;
        SDL_Renderer *renderer_ = nullptr;
        SDL_Event event_;
    };
}