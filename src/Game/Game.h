#include "SDL_mixer.h"
#include "Snake.h"
#include <thread>
#include "SDL.h"

class Game {
public:
    //Start the game and set default settings
    void start();

private:

    //Init music and sounds from SDL Mixer
    void initSDLMixer();

    //Initialize window and shown him
    void initializeSDL();

    void quit();

    void game();

    void clickHandler();

    void upDateWindowTitle();

    void draw();

    Snake snake;
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