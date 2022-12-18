#include "Snake.h"

Snake::Snake() {
    srand(time(0));
}

void Snake::start() {
    is_playing_ = true;
    defaultSnake();
    initializeSDL();
}

void Snake::defaultSnake() {
    dir_ = STOP_DIRECTION;
    length_ = 3;
    score_ = 0;
    tailX_[0] = RECT_X / 2;
    tailY_[0] = RECT_Y / 2;
    fruitX_ = RECT_X / 2 + 5;
    fruitY_ = (RECT_Y / 2) - 5;
    blockX_[0] = RECT_X / 2 - 7;
    blockX_[1] = RECT_X / 2 - 7;
    blockX_[2] = RECT_X / 2 - 6;
    blockX_[3] = RECT_X / 2 - 8;
    blockX_[4] = RECT_X / 2 - 7;
    blockY_[0] = RECT_Y / 2 - 14;
    blockY_[1] = RECT_Y / 2 - 12;
    blockY_[2] = RECT_Y / 2 - 13;
    blockY_[3] = RECT_Y / 2 - 13;
    blockY_[4] = RECT_Y / 2 - 13;
    for (size_t i = 1; i < length_; i++) {
        tailX_[i] = tailX_[i - 1] - 1;
        tailY_[i] = tailY_[i - 1];
    }
}

void Snake::spawnFruit() {
    here:
    fruitX_ = rand() % RECT_X;
    fruitY_ = rand() % RECT_Y;
    for (size_t i = 0; i < 4; i++) {
        if (fruitY_ == blockY_[i] && fruitX_ == blockX_[i])
            goto here;
    }
    for (size_t i = 1; i < length_; i++) {
        if (fruitY_ == tailY_[i] && fruitX_ == tailX_[i])
            goto here;
    }
}

void Snake::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        is_playing_ = false;
    } else {
        window_ = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window_ == nullptr) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            is_playing_ = false;
        } else {
            renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                is_playing_ = false;
            }

            music_ = Mix_LoadMUS("song.wav");
            if (music_ == nullptr) {
                printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
                is_playing_ = false;
            }

            if (Mix_PlayingMusic() == 0) {
                Mix_PlayMusic(music_, -1);
                Mix_VolumeMusic(3);
            }

            game();
        }
    }

    Mix_FreeMusic(music_);
    music_ = nullptr;

    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;

    SDL_DestroyWindow(window_);
    window_ = nullptr;

    SDL_Quit();
}

void Snake::game() {

    Uint32 before, second = SDL_GetTicks(), after;
    int frame_time, frames = 0;

    while (is_playing_) {
        before = SDL_GetTicks();

        //Click Handler
        clickHandler();

        //Moving our snake
        moving();

        //Draw all elements of a map
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

void Snake::upDateWindowTitle() {
    std::string title = "Snake. Score: " + std::to_string(score_) + " FPS: " + std::to_string(fps_);
    SDL_SetWindowTitle(window_, title.c_str());
}

void Snake::draw() {
    SDL_Color color;
    SDL_Rect rect;

    SDL_SetRenderDrawColor(renderer_, 0x63, 0x63, 0x63, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);

    for (size_t i = 0; i < length_; i++) {
        if (i == 0)
            color = {0x00, 0x00, 0xFF};
        else
            color = {0xFF, 0xFF, 0xFF};
        rect = {tailX_[i] * GRID_WIDTH, tailY_[i] * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer_, &rect);
    }

    for (size_t i = 0; i < 5; i++) {
        color = {0x00, 0x00, 0x00};
        rect = {blockX_[i] * GRID_WIDTH, blockY_[i] * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer_, &rect);
    }

    color = {0xFF, 0xFF, 0x00};
    rect = {fruitX_ * GRID_WIDTH, fruitY_ * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer_, &rect);
    SDL_RenderPresent(renderer_);
}

void Snake::clickHandler() {
    SDL_Event event_;
    if (SDL_PollEvent(&event_)) {
        switch (event_.type) {
            case SDL_KEYDOWN:
                switch (event_.key.keysym.sym) {
                    case SDLK_LEFT:
                        dir_ != Direction::RIGHT_DIRECTION ? dir_ = LEFT_DIRECTION : dir_ = RIGHT_DIRECTION;
                        break;
                    case SDLK_RIGHT:
                        dir_ != Direction::LEFT_DIRECTION ? dir_ = RIGHT_DIRECTION : dir_ = LEFT_DIRECTION;
                        break;
                    case SDLK_UP:
                        dir_ != Direction::DOWN_DIRECTION ? dir_ = UP_DIRECTION : dir_ = DOWN_DIRECTION;
                        break;
                    case SDLK_DOWN:
                        dir_ != Direction::UP_DIRECTION ? dir_ = DOWN_DIRECTION : dir_ = UP_DIRECTION;
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
        changeDirection();
    }
}


void Snake::changeDirection() {
    switch (dir_) {
        case LEFT_DIRECTION:
            moveX_ = -1;
            moveY_ = 0;
            break;
        case RIGHT_DIRECTION:
            moveX_ = 1;
            moveY_ = 0;
            break;
        case UP_DIRECTION:
            moveX_ = 0;
            moveY_ = -1;
            break;
        case DOWN_DIRECTION:
            moveX_ = 0;
            moveY_ = 1;
            break;
        case STOP_DIRECTION:
            moveX_ = 0;
            moveY_ = 0;
        default:
            break;
    }
}

void Snake::moving() {
    if (moveX_ != 0 || moveY_ != 0) {
        int prevX = tailX_[0];
        int prevY = tailY_[0];
        tailX_[0] += moveX_;
        tailY_[0] += moveY_;
        endProcess();
        for (size_t i = 1; i < length_; i++) {
            int prev2X = tailX_[i];
            int prev2Y = tailY_[i];
            tailX_[i] = prevX;
            tailY_[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }

        //If snake hit the tail
        plungingCheck();

        //If snake ate a food
        eatingProcess();
    }
}

void Snake::endProcess() {
    if (tailX_[0] < 0) {
        tailX_[0] = RECT_X;
    } else if (tailX_[0] >= RECT_X) {
        tailX_[0] = 0;
    } else if (tailY_[0] < 0) {
        tailY_[0] = RECT_Y;
    } else if (tailY_[0] >= RECT_Y) {
        tailY_[0] = 0;
    }
}

void Snake::eatingProcess() {
    if (tailX_[0] == fruitX_ && tailY_[0] == fruitY_) {
        spawnFruit();
        score_++;
        length_++;
        tailX_[length_ - 1] = tailX_[length_ - 2] - std::abs(moveX_);
        tailY_[length_ - 1] = tailY_[length_ - 2] - std::abs(moveY_);
    }
}

void Snake::plungingCheck() {
    for (size_t i = 0; i < 4; i++) {
        if (tailX_[0] == blockX_[i] && tailY_[0] == blockY_[i]) {
            drawHeadSnake();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            defaultSnake();
        }
    }
    for (size_t i = 1; i < length_; i++) {
        if (tailX_[0] == tailX_[i] && tailY_[0] == tailY_[i]) {
            drawHeadSnake();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            defaultSnake();
        }
    }
}

void Snake::drawHeadSnake() {
    SDL_Color color;
    SDL_Rect rect;
    color = {0xFF, 0x00, 0x00};
    rect = {tailX_[0] * GRID_WIDTH, tailY_[0] * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer_, &rect);
    SDL_RenderPresent(renderer_);
}