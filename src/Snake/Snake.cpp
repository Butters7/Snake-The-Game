#include "Snake.h"

void Snake::defaultSnake(const int &X_, const int &Y_) {
    length_ = 3;
    dir_ = STOP_DIRECTION;
    tail[0] = Point{X_, Y_};
    for (size_t i = 1; i < length_; i++) {
        tail[i] = Point{tail[i - 1].x_ - 1, tail[i - 1].y_};
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
        Point prev = Point{tail[0].x_, tail[0].y_};
        tail[0] = Point{prev.x_ + moveX_, prev.y_ + moveY_};
        endProcess();
        for (size_t i = 1; i < length_; i++) {
            Point prev2 = Point{tail[i].x_, tail[i].y_};
            tail[i] = Point{prev.x_, prev.y_};
            prev = prev2;
        }
    }
}

void Snake::endProcess() {
    if (tail[0].x_ < 0) {
        tail[0].x_ = RECT_X;
    } else if (tail[0].x_ >= RECT_X) {
        tail[0].x_ = 0;
    } else if (tail[0].y_ < 0) {
        tail[0].y_ = RECT_Y;
    } else if (tail[0].y_ >= RECT_Y) {
        tail[0].y_ = 0;
    }
}

void Snake::snakeGrowth() {
    length_++;
    tail[length_ - 1] = Point{tail[length_ - 2].x_ - std::abs(moveX_), tail[length_ - 2].y_ - std::abs(moveY_)};
}

bool Snake::plungingTailCheck() {
    for (size_t i = 1; i < length_; i++) {
        if (tail[0].x_ == tail[i].x_ && tail[0].y_ == tail[i].y_) {
            return true;
        }
    }
    return false;
}