#pragma once

#include "Game.h"

class StartMenu {
public:
    StartMenu();

    bool getNextStep() const;

private:

    void firStart();

    void secStart();

    void thirdStart();

    void initSDL();

    void declareStart();

    void quit();

private:
    char current_pos_;
    bool next_step_;
    bool is_playing_;
    SDL_Rect for_back_;
    SDL_Rect for_start_;
    SDL_Rect for_exit_;
    SDL_Texture *texture_;
    Mix_Music *music_;
    SDL_Window *window_;
    SDL_Renderer *renderer_;
    SDL_Surface *back_start_;
    SDL_Surface *icon_;
    SDL_Surface *u_start_;
    SDL_Surface *d_start_;
    SDL_Surface *u_exit_;
    SDL_Surface *d_exit_;
};