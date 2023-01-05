#pragma once

static int SPEED = 20;
static int FRAME_RATE = 1000 / SPEED;
static const int WIN_SCORE = 20;
static const int MIN_LENGTH = 2;
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