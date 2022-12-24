#pragma once

static const int MIN_LENGTH = 2;
static const int SPEED = 20;
static const int WIN_SCORE = 50;
static const int FRAME_RATE = 1000 / SPEED;
static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 640;
static const int GRID_WIDTH = 20;
static const int GRID_HEIGHT = 20;
static const int RECT_X = SCREEN_WIDTH / GRID_WIDTH;
static const int RECT_Y = SCREEN_HEIGHT / GRID_HEIGHT;

struct Point {
    int x_;
    int y_;
};