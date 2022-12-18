#include "Snake.h"

Snake::Snake() {
    srand(time(0));
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

bool Snake::eatingProcess() {
    if (tailX_[0] == fruitX_ && tailY_[0] == fruitY_) {
        spawnFruit();
        score_++;
        length_++;
        tailX_[length_ - 1] = tailX_[length_ - 2] - std::abs(moveX_);
        tailY_[length_ - 1] = tailY_[length_ - 2] - std::abs(moveY_);
        return true;
    }
    return false;
}

bool Snake::plungingCheck() {
    for (size_t i = 0; i < 4; i++) {
        if (tailX_[0] == blockX_[i] && tailY_[0] == blockY_[i]) {
            return true;
        }
    }
    for (size_t i = 1; i < length_; i++) {
        if (tailX_[0] == tailX_[i] && tailY_[0] == tailY_[i]) {
            return true;
        }
    }
    return false;
}