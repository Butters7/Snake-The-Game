#pragma once

static const int SPEED = 18;
static const int FRAME_RATE = 1000 / SPEED;
static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 640;
static const int GRID_WIDTH = 20;
static const int GRID_HEIGHT = 20;
static const int RECT_X = SCREEN_WIDTH / GRID_WIDTH;
static const int RECT_Y = SCREEN_HEIGHT / GRID_HEIGHT;
static const int COUNT_OF_GRID = RECT_X * RECT_Y;