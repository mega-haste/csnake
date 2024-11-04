#ifndef __GAME_H
#define __GAME_H

#include <raylib.h>
#include <raymath.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

// A length of 100!! I mean.. There's nothing bad will happen... right? RIGHT?
#define MAX_SNAKE_LENGTH 100
#define GRID_SIZE 30
#define GRID_GAP 5

#define APPLE_COUNT 5

#define GRID_ROWS WINDOW_HEIGHT / GRID_SIZE
#define GRID_COLUMNS WINDOW_WIDTH / GRID_SIZE

#define ABSLUTE_POS_OF(vec) Vector2Scale(vec, GRID_SIZE)

// Timer in seconds
#define TIMER 0.2

typedef struct Game {
  Font font;
  bool game_over;

  Vector2 *snake;
  Vector2 snake_vel;
  int current_snake_size;
  float score;

  Vector2 *apples;

  float movement_timer;
} Game;

void game_init(Game *game);
void game_deinit(Game *game);

void game_update(Game *game);
void game_draw(Game *game);

#endif // !__GAME_H
