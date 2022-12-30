#include "Game.h"

void Game::start() {
    srand(time(0));
    snake[0].defaultSnake(0);
    snake[1].defaultSnake(1);
    fruit.cr = Point{RECT_X / 2 + 8, RECT_Y / 2 - 4};
    StartMenu sm;
    if (sm.getNextStep()) {
        initSDLMixer();
        initializeSDL();
        game();
        quit();
    }
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

        countdown_ = Mix_LoadWAV_RW(rWops("COUNTDOWN"), 1);
        if (countdown_ == nullptr) {
            printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
            is_playing_ = false;
        }

        isItMe_ = Mix_LoadWAV_RW(rWops("ISITME"), 1);
        if (isItMe_ == nullptr) {
            printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
            is_playing_ = false;
        } else { Mix_VolumeChunk(isItMe_, 7); }

        eating_ = Mix_LoadWAV_RW(rWops("EAT"), 1);
        if (eating_ == nullptr) {
            printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
            is_playing_ = false;
        } else { Mix_VolumeChunk(eating_, 15); }

        lose_ = Mix_LoadWAV_RW(rWops("LOSE"), 1);
        if (lose_ == nullptr) {
            printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
            is_playing_ = false;
        } else { Mix_VolumeChunk(lose_, 15); }

        Mix_PlayMusic(music_, -1);
        Mix_VolumeMusic(10);
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
            icon_ = SDL_LoadBMP_RW(rWops("SNAKE"), 1);
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

    Mix_FreeChunk(isItMe_);
    isItMe_ = nullptr;

    Mix_FreeChunk(countdown_);
    countdown_ = nullptr;

    Mix_FreeChunk(lose_);
    lose_ = nullptr;

    Mix_FreeChunk(eating_);
    eating_ = nullptr;

    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;

    SDL_DestroyWindow(window_);
    window_ = nullptr;

    SDL_FreeSurface(icon_);
    icon_ = nullptr;

    Mix_Quit();
    SDL_Quit();
}

void Game::game() {
    const int count_of_threads = 7;
    std::thread thread[count_of_threads];

    for (size_t i = 0; i < count_of_threads; i++) {
        switch (i) {
            case 0:
                thread[i] = std::thread([this]() {
                    this->draw();
                });
                break;
            case 1:
                thread[i] = std::thread([this]() {
                    this->isEaten();
                });
                break;
            case 2:
                thread[i] = std::thread([this]() {
                    do {
                        if (this->winner())
                            this->is_playing_ = false;
                    } while (this->is_playing_);
                });
                break;
            case 3:
                thread[i] = std::thread([this]() {
                    this->bigFruitProcessing();
                });
                break;
            case 4:
                thread[i] = std::thread([this]() {
                    this->updateWindowTitle();
                });
                break;
            case 5:
                thread[i] = std::thread([this]() {
                    this->snakeProcessing(0);
                });
                break;
            case 6:
                thread[i] = std::thread([this]() {
                    this->snakeProcessing(1);
                });
                break;
            default:
                break;
        }
    }

    clickHandler();

    for (size_t i = 0; i < count_of_threads; i++) {
        if (thread[i].joinable())
            thread[i].join();
    }

    if (winner())
        initWinner();
}

void Game::isEaten() {
    do {
        if (snake[0].tail[0].x_ == fruit.cr.x_ && snake[0].tail[0].y_ == fruit.cr.y_) {
            snake[0].snakeGrowth();
            spawnFruit();
            Mix_PlayChannel(2, eating_, 0);
        }
        if (snake[1].tail[0].x_ == fruit.cr.x_ && snake[1].tail[0].y_ == fruit.cr.y_) {
            snake[1].snakeGrowth();
            spawnFruit();
            Mix_PlayChannel(2, eating_, 0);
        }
    } while (is_playing_);
}

void Game::clickHandler() {
    SDL_Event event_;
    do {
        while (SDL_PollEvent(&event_)) {
            switch (event_.type) {
                case SDL_KEYDOWN:
                    switch (event_.key.keysym.sym) {
                        case SDLK_LEFT:
                            (snake[0].dir_ != Direction::RIGHT_DIRECTION && snake[0].move.x_ != 1)
                            ? snake[0].dir_ = LEFT_DIRECTION : snake[0].dir_ = RIGHT_DIRECTION;
                            break;
                        case SDLK_RIGHT:
                            (snake[0].dir_ != Direction::LEFT_DIRECTION && snake[0].move.x_ != -1)
                            ? snake[0].dir_ = RIGHT_DIRECTION : snake[0].dir_ = LEFT_DIRECTION;
                            break;
                        case SDLK_UP:
                            (snake[0].dir_ != Direction::DOWN_DIRECTION && snake[0].move.y_ != 1)
                            ? snake[0].dir_ = UP_DIRECTION : snake[0].dir_ = DOWN_DIRECTION;
                            break;
                        case SDLK_DOWN:
                            (snake[0].dir_ != Direction::UP_DIRECTION && snake[0].move.y_ != -1)
                            ? snake[0].dir_ = DOWN_DIRECTION : snake[0].dir_ = UP_DIRECTION;
                            break;
                        case SDLK_a:
                            (snake[1].dir_ != Direction::RIGHT_DIRECTION && snake[1].move.x_ != 1)
                            ? snake[1].dir_ = LEFT_DIRECTION : snake[1].dir_ = RIGHT_DIRECTION;
                            break;
                        case SDLK_d:
                            (snake[1].dir_ != Direction::LEFT_DIRECTION && snake[1].move.x_ != -1)
                            ? snake[1].dir_ = RIGHT_DIRECTION : snake[1].dir_ = LEFT_DIRECTION;
                            break;
                        case SDLK_w:
                            (snake[1].dir_ != Direction::DOWN_DIRECTION && snake[1].move.y_ != 1)
                            ? snake[1].dir_ = UP_DIRECTION : snake[1].dir_ = DOWN_DIRECTION;
                            break;
                        case SDLK_s:
                            (snake[1].dir_ != Direction::UP_DIRECTION && snake[1].move.y_ != -1)
                            ? snake[1].dir_ = DOWN_DIRECTION : snake[1].dir_ = UP_DIRECTION;
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
        snake[0].changeDirection();
        snake[1].changeDirection();
        SDL_Delay(FRAME_RATE);
    } while (is_playing_);
}

void Game::updateWindowTitle() {
    do {
        std::string title =
                "Snake. Blue: " + std::to_string(snake[0].length_ - MIN_LENGTH) + "/" + std::to_string(WIN_SCORE) +
                ". Green: " +
                std::to_string(snake[1].length_ - MIN_LENGTH) + "/" + std::to_string(WIN_SCORE) +
                ". FPS: " + std::to_string(SPEED);
        SDL_SetWindowTitle(window_, title.c_str());
        SDL_Delay(250);
    } while (is_playing_);
}

void Game::draw() {
    do {
        SDL_Color color;
        SDL_Rect rect;

        SDL_SetRenderDrawColor(renderer_, 0x63, 0x63, 0x63, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer_);

        color = {0xFF, 0xFF, 0x00};
        rect = {fruit.cr.x_ * GRID_WIDTH, fruit.cr.y_ * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer_, &rect);

        if (b_fruit.isExisting_) {
            color = {0x7B, 0x68, 0xEE};
            for (size_t i = 0; i < 4; i++) {
                rect = {b_fruit.cr[i].x_ * GRID_WIDTH, b_fruit.cr[i].y_ * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
                SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer_, &rect);
            }
        }

        for (size_t number = 0; number < 2; number++) {
            for (size_t i = 1; i < snake[number].length_; i++) {
                color = {0xFF, 0xFF, 0xFF};
                rect = {snake[number].tail[i].x_ * GRID_WIDTH, snake[number].tail[i].y_ * GRID_HEIGHT, GRID_WIDTH,
                        GRID_HEIGHT};
                SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer_, &rect);
            }

            if (number == 0)
                color = {0x00, 0x00, 0xFF};
            else
                color = {0x00, 0xFF, 0x00};

            rect = {snake[number].tail[0].x_ * GRID_WIDTH, snake[number].tail[0].y_ * GRID_HEIGHT, GRID_WIDTH,
                    GRID_HEIGHT};
            SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer_, &rect);
        }

        for (size_t i = 0; i < block.ct.size(); i++) {
            color = {0x00, 0x00, 0x00};
            rect = {block.ct[i].x_ * GRID_WIDTH, block.ct[i].y_ * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
            SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer_, &rect);
        }

        SDL_RenderPresent(renderer_);
    } while (is_playing_);
}

void Game::spawnFruit() {
    here:
    fruit.cr = Point{rand() % RECT_X, rand() % RECT_X};
    for (size_t i = 0; i < block.ct.size(); i++) {
        if (fruit.cr.y_ == block.ct[i].y_ && fruit.cr.x_ == block.ct[i].x_)
            goto here;
    }
    for (size_t i = 1; i < snake[0].length_; i++) {
        if (fruit.cr.y_ == snake[0].tail[i].y_ && fruit.cr.x_ == snake[0].tail[i].x_)
            goto here;
    }
    for (size_t i = 1; i < snake[1].length_; i++) {
        if (fruit.cr.y_ == snake[1].tail[i].y_ && fruit.cr.x_ == snake[1].tail[i].x_)
            goto here;
    }
}

bool Game::winner() {
    if ((snake[0].length_ - MIN_LENGTH) == WIN_SCORE || (snake[1].length_ - MIN_LENGTH) == WIN_SCORE) {
        return true;
    }
    return false;
}

void Game::initWinner() {
    updateWindowTitle();
    SDL_Surface *image = nullptr;
    SDL_Texture *texture = nullptr;
    Mix_FreeMusic(music_);
    music_ = nullptr;

    if ((snake[0].length_ - MIN_LENGTH) == WIN_SCORE) {
        music_ = Mix_LoadMUS_RW(rWops("BLUE_POWER"), 1);
        image = SDL_LoadBMP_RW(rWops("BP"), 1);
    } else if ((snake[1].length_ - MIN_LENGTH) == WIN_SCORE) {
        music_ = Mix_LoadMUS_RW(rWops("GREEN_POWER"), 1);
        image = SDL_LoadBMP_RW(rWops("GP"), 1);
    }
    if (music_ == nullptr || image == nullptr) {
        printf("Failed to load file! SDL_mixer Error: %s\n", Mix_GetError());
    } else {
        Mix_PlayMusic(music_, -1);
        Mix_VolumeMusic(50);
        texture = SDL_CreateTextureFromSurface(renderer_, image);
        SDL_RenderCopy(renderer_, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer_);

        SDL_Event e;
        is_playing_ = true;
        while (is_playing_) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT)
                    is_playing_ = false;
            }
        }

        SDL_FreeSurface(image);
        image = nullptr;
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Game::snakeProcessing(const int &number) {
    do {
        here:
        if (snake[number].move.x_ != 0 || snake[number].move.y_ != 0) {
            snake[number].moving();
            if (snake[number].plungingTailCheck()) {
                loseProcessing(number);
                if (number == 0)
                    snake[number].defaultSnake(0);
                else
                    snake[number].defaultSnake(1);
                goto here;
            }
            for (size_t i = 0; i < block.ct.size(); i++) {
                if ((i % 5 != 0) &&
                    (snake[number].tail[0].x_ == block.ct[i].x_ && snake[number].tail[0].y_ == block.ct[i].y_)) {
                    loseProcessing(number);
                    if (number == 0)
                        snake[number].defaultSnake(0);
                    else
                        snake[number].defaultSnake(1);
                    goto here;
                }
            }
            if (snake[0].tail[0].x_ == snake[1].tail[0].x_ && snake[0].tail[0].y_ == snake[1].tail[0].y_) {
                b_fruit.lastSnake_ = 'n';
                loseProcessing(-1);
                snake[1].defaultSnake(1);
                snake[0].defaultSnake(0);
                goto here;
            }
            if (number == 0) {
                for (size_t i = 1; i < snake[1].length_; i++) {
                    if (snake[0].tail[0].x_ == snake[1].tail[i].x_ && snake[0].tail[0].y_ == snake[1].tail[i].y_) {
                        loseProcessing(number);
                        snake[0].defaultSnake(0);
                        goto here;
                    }
                }
            } else {
                for (size_t i = 1; i < snake[0].length_; i++) {
                    if (snake[1].tail[0].x_ == snake[0].tail[i].x_ && snake[1].tail[0].y_ == snake[0].tail[i].y_) {
                        loseProcessing(number);
                        snake[1].defaultSnake(1);
                        goto here;
                    }
                }
            }
        }
        SDL_Delay(FRAME_RATE);
    } while (is_playing_);
}

void Game::loseProcessing(const int &number) {
    if (number == (static_cast<int>(b_fruit.lastSnake_) - 48)) {
        b_fruit.lastSnake_ = 'n';
    }
    if (!Mix_Playing(-1)) {
        Mix_PlayChannel(-1, lose_, 0);
    }
}

SDL_RWops *Game::rWops(const std::string &name) {
    HMODULE hModule = nullptr;
    HRSRC resourceInfo = FindResourceA(hModule, name.c_str(), "CUSTOMDATA");
    if (!resourceInfo) {
        fprintf(stderr, "Could not find resource\n");
        return nullptr;
    }
    unsigned int myResourceSize = ::SizeofResource(nullptr, resourceInfo);
    HGLOBAL myResourceData = ::LoadResource(nullptr, resourceInfo);
    void *pMyBinaryData = ::LockResource(myResourceData);

    std::tuple<uint8_t *, std::string, size_t> data((uint8_t *) pMyBinaryData, name, myResourceSize);

    return SDL_RWFromMem(std::get<0>(data), std::get<2>(data));
}

void Game::bigFruitProcessing() {
    do {
        if (b_fruit.lastSnake_ != '0' &&
            ((snake[0].length_ - MIN_LENGTH) == 10 || (snake[0].length_ - MIN_LENGTH) == 15) &&
            (snake[0].length_ - MIN_LENGTH)) {
            b_fruit.lastSnake_ = '0';
            bfp();
        } else if (b_fruit.lastSnake_ != '1' &&
                   ((snake[1].length_ - MIN_LENGTH) == 10 || (snake[1].length_ - MIN_LENGTH) == 15) &&
                   (snake[1].length_ - MIN_LENGTH)) {
            b_fruit.lastSnake_ = '1';
            bfp();
        }
    } while (is_playing_);
}

void Game::bfp() {
    b_fruit.isExisting_ = true;
    b_fruit.cr[0] = Point{rand() % RECT_X, rand() % RECT_Y};
    b_fruit.cr[1] = Point{b_fruit.cr[0].x_ - 1, b_fruit.cr[0].y_};
    b_fruit.cr[2] = Point{b_fruit.cr[0].x_ - 1, b_fruit.cr[0].y_ - 1};
    b_fruit.cr[3] = Point{b_fruit.cr[0].x_, b_fruit.cr[0].y_ - 1};
    auto first = SDL_GetTicks(), second = first;
    Mix_PlayChannel(3, countdown_, 0);
    while (is_playing_ && b_fruit.isExisting_) {
        second = SDL_GetTicks();
        for (size_t i = 0; i < 2; i++) {
            for (size_t j = 0; j < 4; j++) {
                if (snake[i].tail[0].x_ == b_fruit.cr[j].x_ && snake[i].tail[0].y_ == b_fruit.cr[j].y_) {
                    Mix_HaltChannel(3);
                    Mix_VolumeChunk(isItMe_, 15);
                    Mix_PlayChannel(3, isItMe_, 0);
                    snake[i].snakeGrowth();
                    snake[i].snakeGrowth();
                    snake[i].snakeGrowth();
                    snake[i].snakeGrowth();
                    b_fruit.isExisting_ = false;
                    break;
                }
            }
        }
        if (second - first >= 5000) {
            b_fruit.isExisting_ = false;
        }
    }
}