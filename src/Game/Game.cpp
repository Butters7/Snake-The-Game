#include "Game.h"

void Game::start() {
    srand(time(0));
    is_playing_ = true;
    snake.defaultSnake();
    initSDLMixer();
    initializeSDL();
    game();
    quit();
}

void Game::initSDLMixer() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        is_playing_ = false;
    } else {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
            is_playing_ = false;
        }

        music_ = Mix_LoadMUS("../Files/song.wav");
        if (music_ == nullptr) {
            printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
            is_playing_ = false;
        }

        eating_ = Mix_LoadWAV("../Files/eat.wav");
        if (eating_ == nullptr) {
            printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
            is_playing_ = false;
        }

        lose_ = Mix_LoadWAV("../Files/lose.wav");
        if (lose_ == nullptr) {
            printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
            is_playing_ = false;
        } else { Mix_VolumeChunk(lose_, 7); }

        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic(music_, -1);
            Mix_VolumeMusic(3);
        }
    }
}

void Game::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        is_playing_ = false;
    } else {
        window_ = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window_ == nullptr) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            is_playing_ = false;
        } else {
            icon_ = SDL_LoadBMP("../Files/snake.bmp");
            if (icon_ == nullptr) {
                printf("Could not to load icon image! SDL_Error: %s\n", SDL_GetError());
                is_playing_ = false;
            }
            SDL_SetWindowIcon(window_, icon_);
            renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
        }
    }
}

void Game::quit() {
    Mix_FreeMusic(music_);
    music_ = nullptr;

    Mix_FreeChunk(lose_);
    lose_ = nullptr;

    Mix_FreeChunk(eating_);
    eating_ = nullptr;

    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;

    SDL_DestroyWindow(window_);
    window_ = nullptr;

    Mix_Quit();
    SDL_Quit();
}

void Game::game() {
    Uint32 before, second = SDL_GetTicks(), after;
    int frame_time, frames = 0;

    while (is_playing_) {
        before = SDL_GetTicks();

        clickHandler();

        snake.moving();

        if (snake.plungingCheck()) {
            Mix_FreeMusic(music_);
            Mix_PlayChannel(-1, lose_, 0);
            music_ = nullptr;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            snake.defaultSnake();
            initSDLMixer();
        }

        if (snake.eatingProcess()) {
            Mix_PlayChannel(-1, eating_, 0);
        }

        draw();

        frames++;
        after = SDL_GetTicks();
        frame_time = after - before;

        if (after - second >= 1000) {
            fps_ = frames;
            frames = 0;
            second = after;
            upDateWindowTitle();
        }

        if (FRAME_RATE > frame_time) {
            SDL_Delay(FRAME_RATE - frame_time);
        }
    }
}

void Game::clickHandler() {
    while (SDL_PollEvent(&event_)) {
        switch (event_.type) {
            case SDL_KEYDOWN:
                switch (event_.key.keysym.sym) {
                    case SDLK_LEFT:
                        (snake.dir_ != Direction::RIGHT_DIRECTION && snake.dir_ != Direction::STOP_DIRECTION &&
                         snake.moveX_ != 1)
                        ? snake.dir_ = LEFT_DIRECTION : snake.dir_ = RIGHT_DIRECTION;
                        break;
                    case SDLK_RIGHT:
                        (snake.dir_ != Direction::LEFT_DIRECTION && snake.moveX_ != -1)
                        ? snake.dir_ = RIGHT_DIRECTION : snake.dir_ = LEFT_DIRECTION;
                        break;
                    case SDLK_UP:
                        (snake.dir_ != Direction::DOWN_DIRECTION && snake.moveY_ != 1)
                        ? snake.dir_ = UP_DIRECTION : snake.dir_ = DOWN_DIRECTION;
                        break;
                    case SDLK_DOWN:
                        (snake.dir_ != Direction::UP_DIRECTION && snake.moveY_ != -1)
                        ? snake.dir_ = DOWN_DIRECTION : snake.dir_ = UP_DIRECTION;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                is_playing_ = false;
                break;
            default:
                break;
        }
    }
    snake.changeDirection();
}

void Game::upDateWindowTitle() {
    std::string title = "Snake. Score: " + std::to_string(snake.score_) + " FPS: " + std::to_string(fps_);
    SDL_SetWindowTitle(window_, title.c_str());
}

void Game::draw() {
    SDL_Color color;
    SDL_Rect rect;

    SDL_SetRenderDrawColor(renderer_, 0x63, 0x63, 0x63, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);

    for (size_t i = 0; i < snake.length_; i++) {
        if (i == 0)
            color = {0x00, 0x00, 0xFF};
        else
            color = {0xFF, 0xFF, 0xFF};
        rect = {snake.tailX_[i] * GRID_WIDTH, snake.tailY_[i] * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer_, &rect);
    }

    for (size_t i = 0; i < 5; i++) {
        color = {0x00, 0x00, 0x00};
        rect = {snake.blockX_[i] * GRID_WIDTH, snake.blockY_[i] * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer_, &rect);
    }

    color = {0xFF, 0xFF, 0x00};
    rect = {snake.fruitX_ * GRID_WIDTH, snake.fruitY_ * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer_, &rect);
    SDL_RenderPresent(renderer_);
}