#include "StartMenu.h"

StartMenu::StartMenu() {
    next_step_ = false;
    is_playing_ = true;
    icon_ = nullptr;
    music_ = nullptr;
    window_ = nullptr;
    renderer_ = nullptr;
    back_ = nullptr;
    u_start_ = nullptr;
    d_start_ = nullptr;
    u_exit_ = nullptr;
    d_exit_ = nullptr;
    texture_ = nullptr;
    for_start_.x = 125;
    for_start_.y = -75;
    for_start_.w = 640;
    for_start_.h = 640;
    for_exit_.x = 125;
    for_exit_.y = 50;
    for_exit_.w = 640;
    for_exit_.h = 640;
    initSDL();
}

void StartMenu::declareStart() {

    SDL_SetWindowIcon(window_, icon_);
    renderer_ = SDL_CreateRenderer(window_, -1, 0);
    texture_ = SDL_CreateTextureFromSurface(renderer_, back_);
    SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
    SDL_DestroyTexture(texture_);
    texture_ = SDL_CreateTextureFromSurface(renderer_, u_start_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_start_);
    SDL_DestroyTexture(texture_);
    texture_ = SDL_CreateTextureFromSurface(renderer_, u_exit_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_exit_);
    SDL_DestroyTexture(texture_);
    SDL_RenderPresent(renderer_);
    Mix_PlayMusic(music_, -1);
    Mix_VolumeMusic(3);

    SDL_Event e;
    while (is_playing_) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            texture_ = SDL_CreateTextureFromSurface(renderer_, back_);
                            SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
                            SDL_DestroyTexture(texture_);
                            texture_ = SDL_CreateTextureFromSurface(renderer_, d_start_);
                            SDL_RenderCopy(renderer_, texture_, nullptr, &for_start_);
                            SDL_DestroyTexture(texture_);
                            texture_ = SDL_CreateTextureFromSurface(renderer_, u_exit_);
                            SDL_RenderCopy(renderer_, texture_, nullptr, &for_exit_);
                            SDL_DestroyTexture(texture_);
                            next_step_ = true;
                            break;
                        case SDLK_DOWN:
                            texture_ = SDL_CreateTextureFromSurface(renderer_, back_);
                            SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
                            SDL_DestroyTexture(texture_);
                            texture_ = SDL_CreateTextureFromSurface(renderer_, u_start_);
                            SDL_RenderCopy(renderer_, texture_, nullptr, &for_start_);
                            SDL_DestroyTexture(texture_);
                            texture_ = SDL_CreateTextureFromSurface(renderer_, d_exit_);
                            SDL_RenderCopy(renderer_, texture_, nullptr, &for_exit_);
                            SDL_DestroyTexture(texture_);
                            next_step_ = false;
                            break;
                        case SDLK_RETURN:
                            is_playing_ = false;
                            break;
                    }
                    break;
                case SDL_QUIT:
                    is_playing_ = false;
                    break;
            }
            SDL_RenderPresent(renderer_);
        }
    }
}

void StartMenu::quit() {
    SDL_FreeSurface(icon_);
    icon_ = nullptr;
    SDL_DestroyWindow(window_);
    window_ = nullptr;
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
    Mix_FreeMusic(music_);
    music_ = nullptr;
    SDL_FreeSurface(back_);
    back_ = nullptr;
    SDL_FreeSurface(u_start_);
    u_start_ = nullptr;
    SDL_FreeSurface(d_start_);
    d_start_ = nullptr;
    SDL_FreeSurface(u_exit_);
    u_exit_ = nullptr;
    SDL_FreeSurface(d_exit_);
    d_exit_ = nullptr;
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;

    if (!next_step_) {
        Mix_Quit();
        SDL_Quit();
    }
}

bool StartMenu::getNextStep() const {
    return next_step_;
}

void StartMenu::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        window_ = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window_ == nullptr || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        } else {
            music_ = Mix_LoadMUS_RW(Game::rWops("SONG_START"), 1);
            back_ = SDL_LoadBMP_RW(Game::rWops("PIC_START"), 1);
            u_start_ = SDL_LoadBMP_RW(Game::rWops("U_START"), 1);
            d_start_ = SDL_LoadBMP_RW(Game::rWops("D_START"), 1);
            u_exit_ = SDL_LoadBMP_RW(Game::rWops("U_EXIT"), 1);
            d_exit_ = SDL_LoadBMP_RW(Game::rWops("D_EXIT"), 1);
            icon_ = SDL_LoadBMP_RW(Game::rWops("SNAKE"), 1);

            if (!music_ || !back_ || !u_start_ || !d_start_ || !u_exit_ || !d_exit_ || !icon_) {
                printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            } else {
                declareStart();
                quit();
            }
        }
    }
}