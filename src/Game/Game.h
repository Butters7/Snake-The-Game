#pragma once

#include "../Snake/Snake.h"
#include "../Block/Block.h"
#include "../Fruit/Fruit.h"
#include "SDL_mixer.h"
#include "Windows.h"
#include <thread>
#include "SDL.h"

class Game {
public:
    //Start the game and set default settings
    void start();

private:

    //Default settings of our snake
    void defaultSettings();

    //Init music and sounds from SDL Mixer
    void initSDLMixer();

    //Initialize window and shown him
    void initializeSDL();

    //Checking if the snake hit the tail or block
    void plungingCheck();

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

    //Loading our resources from rc file
    static SDL_RWops *rWops(const std::string &name);

    //Clear memory and exit SDL
    void quit();

    Snake snake1;
    Fruit fruit;
    Block block;
    bool is_playing_;
    int fps_;
    SDL_Event event_;
    Mix_Music *music_ = nullptr;
    Mix_Chunk *eating_ = nullptr;
    Mix_Chunk *lose_ = nullptr;
    SDL_Surface *icon_ = nullptr;
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
};