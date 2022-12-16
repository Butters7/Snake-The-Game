#include "Snake.h"

using namespace GAME;

Snake::Snake() {
    srand(time(0));
}

void Snake::start() {
    isPlaying_ = true;
    defaultSnake();
    initializeSDL();
}

void Snake::defaultSnake() {
    moveX_ = 0;
    moveY_ = 0;
    length_ = 3;
    score_ = 0;
    tailX_[0] = RECT_X / 2;
    tailY_[0] = RECT_Y / 2;
    for (size_t i = 1; i < length_; i++) {
        tailX_[i] = tailX_[i - 1] - 1;
        tailY_[i] = tailY_[i - 1];
    }
    spawnFruit();
}

void Snake::spawnFruit() {
    here:
    fruitX_ = rand() % RECT_X;
    fruitY_ = rand() % RECT_Y;
    for (size_t i = 1; i < length_; i++) {
        if (fruitY_ == tailY_[i] && fruitX_ == tailX_[i])
            goto here;
    }
}

void Snake::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        isPlaying_ = false;
    } else {
        window_ = SDL_CreateWindow("Snake The Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window_ == nullptr) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            isPlaying_ = false;
        } else {
            game();
        }
    }

    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;

    SDL_DestroyWindow(window_);
    window_ = nullptr;

    SDL_Quit();
}

void Snake::game() {
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    while (isPlaying_) {
        //Draw all elements of a map
        draw();

        //Moving our snake
        if (moveX_ != 0 || moveY_ != 0) {
            moving();
        }

        //If snake ate a food
        if (tailX_[0] == fruitX_ && tailY_[0] == fruitY_) {
            eatingProcess();
        }

        //If snake hit the tail
        plungingCheck();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Snake::draw() {
    SDL_Color color;
    SDL_Rect rect;

    if (SDL_PollEvent(&event_)) {
        clickHandler();
        changeDirection();
    }

    SDL_SetRenderDrawColor(renderer_, 0x63, 0x63, 0x63, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);

    for (size_t X = 0; X < SCREEN_WIDTH; X += GRID_WIDTH) {
        SDL_SetRenderDrawColor(renderer_, 0x0, 0x0, 0x0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer_, X, 0, X, SCREEN_HEIGHT);
        SDL_RenderPresent(renderer_);

    }

    for (size_t Y = 0; Y < SCREEN_HEIGHT; Y += GRID_HEIGHT) {
        SDL_SetRenderDrawColor(renderer_, 0x0, 0x0, 0x0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer_, 0, Y, SCREEN_WIDTH, Y);
        SDL_RenderPresent(renderer_);

    }

    for (size_t i = 0; i < length_; i++) {
        if (i == 0)
            color = {0xFF, 0x00, 0x00};
        else
            color = {0xFF, 0xFF, 0xFF};
        rect = {tailX_[i] * GRID_WIDTH, tailY_[i] * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer_, &rect);
        SDL_RenderPresent(renderer_);
    }

    color = {0x00, 0xFF, 0x00};
    rect = {fruitX_ * GRID_WIDTH, fruitY_ * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer_, &rect);
    SDL_RenderPresent(renderer_);
}

void Snake::clickHandler() {
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
            isPlaying_ = false;
            break;
        default:
            break;
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
        default:
            break;
    }
}

void Snake::moving() {
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
    spawnFruit();
    score_++;
    length_++;
    tailX_[length_ - 1] = tailX_[length_ - 2] - std::abs(moveX_);
    tailY_[length_ - 1] = tailY_[length_ - 2] - std::abs(moveY_);
}

void Snake::plungingCheck() {
    for (size_t i = 1; i < length_; i++) {
        if (tailX_[0] == tailX_[i] && tailY_[0] == tailY_[i])
            defaultSnake();
    }
}