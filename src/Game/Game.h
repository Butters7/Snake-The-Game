#pragma once

#include "../Snake/Snake.h"
#include "../Block/Block.h"
#include "../Fruit/Fruit.h"
#include <string>
#include "SDL_mixer.h"
#include "Windows.h"
#include <thread>
#include "SDL.h"

class Game {
public:
    //Start the game and set default settings
    void start();

    Game() {
        is_playing_ = true;
        music_ = nullptr;
        eating_ = nullptr;
        lose_ = nullptr;
        icon_ = nullptr;
        window_ = nullptr;
        renderer_ = nullptr;
    }

private:
    //Init music and sounds from SDL Mixer
    void initSDLMixer();

    //Initialize window and shown him
    void initializeSDL();

    //Checking if the snake hit the tail or block or other snake
    void snakeProcessing(const int &number);

    //If lose then play chunk
    void loseProcessing();

    //Spawn fruit until he appears not in a snake or in a block
    void spawnFruit();

    //Check if snake ate the food
    void isEaten();

    //Starting the game
    void game();

    //Intercept keystrokes and process them
    void clickHandler();

    //UpdateWindowTitle
    void updateWindowTitle();

    //Draw snake, blocks, fruit, and field
    void draw();

    //Clear memory and exit SDL
    void quit();

    //Check has anyone reached 50 points
    bool winner();

    //Initialize bmp file with winner
    void initWinner();

    //Loading our resources from rc file
    static SDL_RWops *rWops(const std::string &name);

    Snake snake[2];
    Fruit fruit;
    Block block;
    bool is_playing_;
    Mix_Music *music_;
    Mix_Chunk *eating_;
    Mix_Chunk *lose_;
    SDL_Surface *icon_;
    SDL_Window *window_;
    SDL_Renderer *renderer_;
};