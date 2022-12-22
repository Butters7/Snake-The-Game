#include "Snake.h"

void Snake::defaultSnake(const int &X_, const int &Y_) {
    length_ = 3;
    dir_ = STOP_DIRECTION;
    tailX_[0] = X_ / 2;
    tailY_[0] = Y_ / 2;
    for (size_t i = 1; i < length_; i++) {
        tailX_[i] = tailX_[i - 1] - 1;
        tailY_[i] = tailY_[i - 1];
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

void Snake::snakeGrowth() {
    length_++;
    tailX_[length_ - 1] = tailX_[length_ - 2] - std::abs(moveX_);
    tailY_[length_ - 1] = tailY_[length_ - 2] - std::abs(moveY_);
}

bool Snake::plungingTailCheck() {
    for (size_t i = 1; i < length_; i++) {
        if (tailX_[0] == tailX_[i] && tailY_[0] == tailY_[i]) {
            return true;
        }
    }
    return false;
}