#include "StartMenu.h"

StartMenu::StartMenu() {
    complexity_ = 0;
    current_pos_ = 0;
    next_step_ = false;
    is_playing_ = true;
    icon_ = nullptr;
    music_ = nullptr;
    window_ = nullptr;
    renderer_ = nullptr;
    back_start_ = nullptr;
    u_start_ = nullptr;
    d_start_ = nullptr;
    u_easy_ = nullptr;
    d_easy_ = nullptr;
    u_hard_ = nullptr;
    d_hard_ = nullptr;
    u_exit_ = nullptr;
    d_exit_ = nullptr;
    texture_ = nullptr;
    for_back_ = SDL_Rect{0, 0, 640, 640};
    for_start_ = SDL_Rect{350, 150, 200, 120};
    for_hard_ = SDL_Rect{310, 245, 270, 135};
    for_easy_ = SDL_Rect{350, 250, 200, 120};
    for_exit_ = SDL_Rect{350, 350, 196, 120};
    initSDL();
}

void StartMenu::declareStart() {

    SDL_SetWindowIcon(window_, icon_);
    renderer_ = SDL_CreateRenderer(window_, -1, 0);
    firStart();
    SDL_RenderPresent(renderer_);
    Mix_PlayMusic(music_, -1);
    Mix_VolumeMusic(15);

    int xMouse = 0, yMouse = 0;

    SDL_Event e;
    while (is_playing_) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_MOUSEMOTION:
                    SDL_GetMouseState(&xMouse, &yMouse);
                    if (xMouse > 350 && xMouse < 550 && yMouse > 170 && yMouse < 240)
                        secStart();
                    else if (xMouse > 350 && xMouse < 550 && yMouse > 270 && yMouse < 340)
                        fourthStart();
                    else if (xMouse > 350 && xMouse < 550 && yMouse > 370 && yMouse < 440)
                        thirdStart();
                    else
                        firStart();
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (current_pos_ == 'p') {
                        next_step_ = true;
                        is_playing_ = false;
                    } else if (current_pos_ == 'e') {
                        is_playing_ = false;
                        next_step_ = false;
                    } else if (current_pos_ == 'c')
                        if (complexity_ != 'h') {
                            complexity_ = 'h';
                            fourthStart();
                        } else {
                            complexity_ = 'e';
                            fourthStart();
                        }
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            if (current_pos_ == 0 || current_pos_ == 'c')
                                secStart();
                            else if (current_pos_ == 'e')
                                fourthStart();
                            else
                                thirdStart();
                            break;
                        case SDLK_DOWN:
                            if (current_pos_ == 0 || current_pos_ == 'c')
                                thirdStart();
                            else if (current_pos_ == 'e')
                                secStart();
                            else
                                fourthStart();
                            break;
                        case SDLK_RETURN:
                            if (current_pos_ == 'p') {
                                next_step_ = true;
                                is_playing_ = false;
                            } else if (current_pos_ == 'e') {
                                next_step_ = false;
                                is_playing_ = false;
                            } else if (current_pos_ == 'c')
                                if (complexity_ != 'h') {
                                    complexity_ = 'h';
                                    fourthStart();
                                } else {
                                    complexity_ = 'e';
                                    fourthStart();
                                }
                            break;
                    }
                    break;
                case SDL_QUIT:
                    next_step_ = false;
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
    SDL_FreeSurface(back_start_);
    back_start_ = nullptr;
    SDL_FreeSurface(u_start_);
    u_start_ = nullptr;
    SDL_FreeSurface(d_start_);
    d_start_ = nullptr;
    SDL_FreeSurface(d_easy_);
    d_easy_ = nullptr;
    SDL_FreeSurface(u_easy_);
    u_easy_ = nullptr;
    SDL_FreeSurface(d_hard_);
    d_hard_ = nullptr;
    SDL_FreeSurface(u_hard_);
    u_hard_ = nullptr;
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

bool StartMenu::isHard() const {
    if (complexity_ == 'h')
        return true;
    else
        return false;
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
            back_start_ = SDL_LoadBMP_RW(Game::rWops("PIC_START"), 1);
            u_start_ = SDL_LoadBMP_RW(Game::rWops("U_START"), 1);
            d_start_ = SDL_LoadBMP_RW(Game::rWops("D_START"), 1);
            d_easy_ = SDL_LoadBMP_RW(Game::rWops("U_EASY"), 1);
            u_easy_ = SDL_LoadBMP_RW(Game::rWops("D_EASY"), 1);
            d_hard_ = SDL_LoadBMP_RW(Game::rWops("U_HARD"), 1);
            u_hard_ = SDL_LoadBMP_RW(Game::rWops("D_HARD"), 1);
            u_exit_ = SDL_LoadBMP_RW(Game::rWops("U_EXIT"), 1);
            d_exit_ = SDL_LoadBMP_RW(Game::rWops("D_EXIT"), 1);
            icon_ = SDL_LoadBMP_RW(Game::rWops("SNAKE"), 1);

            if (!music_ || !back_start_ || !u_start_ || !d_start_ || !u_exit_ || !d_exit_ || !icon_ || !d_hard_ ||
                !d_easy_ || !u_hard_ || !u_easy_) {
                printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            } else {
                declareStart();
                quit();
            }
        }
    }
}

void StartMenu::firStart() {
    texture_ = SDL_CreateTextureFromSurface(renderer_, back_start_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_back_);
    SDL_DestroyTexture(texture_);
    texture_ = SDL_CreateTextureFromSurface(renderer_, u_start_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_start_);
    SDL_DestroyTexture(texture_);\
    if (complexity_ != 'h') {
        texture_ = SDL_CreateTextureFromSurface(renderer_, u_easy_);
        SDL_RenderCopy(renderer_, texture_, nullptr, &for_easy_);
    } else {
        texture_ = SDL_CreateTextureFromSurface(renderer_, u_hard_);
        SDL_RenderCopy(renderer_, texture_, nullptr, &for_hard_);
    }
    SDL_DestroyTexture(texture_);
    texture_ = SDL_CreateTextureFromSurface(renderer_, u_exit_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_exit_);
    SDL_DestroyTexture(texture_);
    current_pos_ = 0;
}

void StartMenu::secStart() {
    texture_ = SDL_CreateTextureFromSurface(renderer_, back_start_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_back_);
    SDL_DestroyTexture(texture_);
    texture_ = SDL_CreateTextureFromSurface(renderer_, d_start_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_start_);
    SDL_DestroyTexture(texture_);
    if (complexity_ != 'h') {
        texture_ = SDL_CreateTextureFromSurface(renderer_, u_easy_);
        SDL_RenderCopy(renderer_, texture_, nullptr, &for_easy_);
    } else {
        texture_ = SDL_CreateTextureFromSurface(renderer_, u_hard_);
        SDL_RenderCopy(renderer_, texture_, nullptr, &for_hard_);
    }
    SDL_DestroyTexture(texture_);
    texture_ = SDL_CreateTextureFromSurface(renderer_, u_exit_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_exit_);
    SDL_DestroyTexture(texture_);
    current_pos_ = 'p';
}

void StartMenu::thirdStart() {
    texture_ = SDL_CreateTextureFromSurface(renderer_, back_start_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_back_);
    SDL_DestroyTexture(texture_);
    texture_ = SDL_CreateTextureFromSurface(renderer_, u_start_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_start_);
    SDL_DestroyTexture(texture_);
    if (complexity_ != 'h') {
        texture_ = SDL_CreateTextureFromSurface(renderer_, u_easy_);
        SDL_RenderCopy(renderer_, texture_, nullptr, &for_easy_);
    } else {
        texture_ = SDL_CreateTextureFromSurface(renderer_, u_hard_);
        SDL_RenderCopy(renderer_, texture_, nullptr, &for_hard_);
    }
    SDL_DestroyTexture(texture_);
    texture_ = SDL_CreateTextureFromSurface(renderer_, d_exit_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_exit_);
    SDL_DestroyTexture(texture_);
    current_pos_ = 'e';
}

void StartMenu::fourthStart() {
    texture_ = SDL_CreateTextureFromSurface(renderer_, back_start_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_back_);
    SDL_DestroyTexture(texture_);
    texture_ = SDL_CreateTextureFromSurface(renderer_, u_start_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_start_);
    SDL_DestroyTexture(texture_);
    if (complexity_ != 'h') {
        texture_ = SDL_CreateTextureFromSurface(renderer_, d_easy_);
        SDL_RenderCopy(renderer_, texture_, nullptr, &for_easy_);
    } else {
        texture_ = SDL_CreateTextureFromSurface(renderer_, d_hard_);
        SDL_RenderCopy(renderer_, texture_, nullptr, &for_hard_);
    }
    SDL_DestroyTexture(texture_);
    texture_ = SDL_CreateTextureFromSurface(renderer_, u_exit_);
    SDL_RenderCopy(renderer_, texture_, nullptr, &for_exit_);
    SDL_DestroyTexture(texture_);
    current_pos_ = 'c';
}