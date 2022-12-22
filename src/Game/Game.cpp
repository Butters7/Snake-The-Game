#include "Game.h"

void Game::start() {
    srand(time(0));
    is_playing_ = true;
    defaultSettings();
    initSDLMixer();
    initializeSDL();
    game();
    quit();
}

void Game::defaultSettings() {
    block1.x_[0] = RECT_X / 2 - 7;
    block1.x_[1] = RECT_X / 2 - 7;
    block1.x_[2] = RECT_X / 2 - 6;
    block1.x_[3] = RECT_X / 2 - 8;
    block1.x_[4] = RECT_X / 2 - 7;
    block1.y_[0] = RECT_Y / 2 - 14;
    block1.y_[1] = RECT_Y / 2 - 12;
    block1.y_[2] = RECT_Y / 2 - 13;
    block1.y_[3] = RECT_Y / 2 - 13;
    block1.y_[4] = RECT_Y / 2 - 13;
    fruit.x_ = RECT_X / 2 + 8;
    fruit.y_ = RECT_Y / 2 - 4;
    snake1.defaultSnake(RECT_X / 2, RECT_Y / 2);
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

        music_ = Mix_LoadMUS_RW(rWops("SONG"), 1);
        if (music_ == nullptr) {
            printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
            is_playing_ = false;
        }

        eating_ = Mix_LoadWAV_RW(rWops("EAT"), 1);
        if (eating_ == nullptr) {
            printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
            is_playing_ = false;
        }

        lose_ = Mix_LoadWAV_RW(rWops("LOSE"), 1);
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
            icon_ = SDL_LoadBMP_RW(rWops("ICON"), 1);
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

        snake1.moving();

        plungingCheck();

        isEaten();

        draw();

        frames++;
        after = SDL_GetTicks();
        frame_time = after - before;

        if (after - second >= 1000) {
            fps_ = frames;
            frames = 0;
            second = after;
            updateWindowTitle();
        }

        if (FRAME_RATE > frame_time) {
            SDL_Delay(FRAME_RATE - frame_time);
        }
    }
}

void Game::isEaten() {
    if (snake1.tailX_[0] == fruit.x_ && snake1.tailY_[0] == fruit.y_) {
        snake1.snakeGrowth();
        spawnFruit();
        Mix_PlayChannel(-1, eating_, 0);
    }
}

void Game::clickHandler() {
    while (SDL_PollEvent(&event_)) {
        switch (event_.type) {
            case SDL_KEYDOWN:
                switch (event_.key.keysym.sym) {
                    case SDLK_LEFT:
                        (snake1.dir_ != Direction::RIGHT_DIRECTION && snake1.dir_ != Direction::STOP_DIRECTION &&
                         snake1.moveX_ != 1)
                        ? snake1.dir_ = LEFT_DIRECTION : snake1.dir_ = RIGHT_DIRECTION;
                        break;
                    case SDLK_RIGHT:
                        (snake1.dir_ != Direction::LEFT_DIRECTION && snake1.moveX_ != -1)
                        ? snake1.dir_ = RIGHT_DIRECTION : snake1.dir_ = LEFT_DIRECTION;
                        break;
                    case SDLK_UP:
                        (snake1.dir_ != Direction::DOWN_DIRECTION && snake1.moveY_ != 1)
                        ? snake1.dir_ = UP_DIRECTION : snake1.dir_ = DOWN_DIRECTION;
                        break;
                    case SDLK_DOWN:
                        (snake1.dir_ != Direction::UP_DIRECTION && snake1.moveY_ != -1)
                        ? snake1.dir_ = DOWN_DIRECTION : snake1.dir_ = UP_DIRECTION;
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
    snake1.changeDirection();
}

void Game::updateWindowTitle() {
    std::string title = "Snake. Score: " + std::to_string(snake1.length_ - 3) + " FPS: " + std::to_string(fps_);
    SDL_SetWindowTitle(window_, title.c_str());
}

void Game::draw() {
    SDL_Color color;
    SDL_Rect rect;

    SDL_SetRenderDrawColor(renderer_, 0x63, 0x63, 0x63, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);

    for (size_t i = 0; i < snake1.length_; i++) {
        if (i == 0)
            color = {0x00, 0x00, 0xFF};
        else
            color = {0xFF, 0xFF, 0xFF};
        rect = {snake1.tailX_[i] * GRID_WIDTH, snake1.tailY_[i] * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer_, &rect);
    }

    for (size_t i = 0; i < 5; i++) {
        color = {0x00, 0x00, 0x00};
        rect = {block1.x_[i] * GRID_WIDTH, block1.y_[i] * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer_, &rect);
    }

    color = {0xFF, 0xFF, 0x00};
    rect = {fruit.x_ * GRID_WIDTH, fruit.y_ * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer_, &rect);
    SDL_RenderPresent(renderer_);
}

SDL_RWops *Game::rWops(const std::string &name) {
    HMODULE hModule = NULL;
    HRSRC resourceInfo = FindResourceA(hModule, name.c_str(), "CUSTOMDATA");
    if (!resourceInfo) {
        fprintf(stderr, "Could not find resource\n");
        return nullptr;
    }
    unsigned int myResourceSize = ::SizeofResource(NULL, resourceInfo);
    HGLOBAL myResourceData = ::LoadResource(NULL, resourceInfo);
    void *pMyBinaryData = ::LockResource(myResourceData);

    std::tuple<uint8_t *, std::string, size_t> data((uint8_t *) pMyBinaryData, name, myResourceSize);

    return SDL_RWFromMem(std::get<0>(data), std::get<2>(data));
}

void Game::spawnFruit() {
    here:
    fruit.x_ = rand() % RECT_X;
    fruit.y_ = rand() % RECT_Y;
    for (size_t i = 0; i < 5; i++) {
        if (fruit.y_ == block1.y_[i] && fruit.x_ == block1.x_[i])
            goto here;
    }
    for (size_t i = 1; i < snake1.length_; i++) {
        if (fruit.y_ == snake1.tailY_[i] && fruit.x_ == snake1.tailX_[i])
            goto here;
    }
}

void Game::plungingCheck() {
    for (size_t i = 0; i < 4; i++) {
        if (snake1.plungingTailCheck() || (snake1.tailX_[0] == block1.x_[i] && snake1.tailY_[0] == block1.y_[i])) {
            Mix_FreeMusic(music_);
            Mix_PlayChannel(-1, lose_, 0);
            music_ = nullptr;
            SDL_Delay(1000);
            snake1.defaultSnake(RECT_X / 2, RECT_Y / 2);
            SDL_QuitEvent(event_);
            defaultSettings();
            initSDLMixer();
        }
    }
}