#include "Snake.h"

void Snake::defaultSnake(const int &number) {
    length_ = MIN_LENGTH;
    move = Point{0, 0};
    dir_ = STOP_DIRECTION;
    tail.fill(Point{0, 0});
    if (number == 0)
        tail[0] = Point{0, RECT_Y / 2 - 2};
    else
        tail[0] = Point{0, RECT_Y / 2 + 2};

    for (size_t i = 1; i < length_; i++) {
        tail[i] = Point{tail[i - 1].x_, tail[i - 1].y_};
    }
}

void Snake::changeDirection() {
    switch (dir_) {
        case LEFT_DIRECTION:
            move = Point{-1, 0};
            break;
        case RIGHT_DIRECTION:
            move = Point{1, 0};
            break;
        case UP_DIRECTION:
            move = Point{0, -1};
            break;
        case DOWN_DIRECTION:
            move = Point{0, 1};
            break;
        default:
            break;
    }
}

void Snake::moving() {
    if (move.x_ != 0 || move.y_ != 0) {
        auto prev = Point{tail[0].x_, tail[0].y_};
        tail[0] = Point{prev.x_ + move.x_, prev.y_ + move.y_};
        endProcess();
        for (size_t i = 1; i < length_; i++) {
            auto prev2 = Point{tail[i].x_, tail[i].y_};
            tail[i] = Point{prev.x_, prev.y_};
            prev = prev2;
        }
    }
}

void Snake::endProcess() {
    if (tail[0].x_ < 0) {
        tail[0].x_ = RECT_X - 1;
    } else if (tail[0].x_ > (RECT_X - 1)) {
        tail[0].x_ = 0;
    } else if (tail[0].y_ < 0) {
        tail[0].y_ = RECT_Y - 1;
    } else if (tail[0].y_ > (RECT_Y - 1)) {
        tail[0].y_ = 0;
    }
}

void Snake::snakeGrowth() {
    length_++;
    tail[length_ - 1] = Point{tail[length_ - 2].x_ - std::abs(move.x_), tail[length_ - 2].y_ - std::abs(move.y_)};
}

bool Snake::plungingTailCheck() {
    for (size_t i = 1; i < length_; i++) {
        if (tail[0].x_ == tail[i].x_ && tail[0].y_ == tail[i].y_) {
            return true;
        }
    }
    return false;
}