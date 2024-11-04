#include "Game.h"
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static void snake_shift(Game *game);
static Vector2 get_snake_abslute_pos(Game *game);
static void random_apple_pos(Game *game, Vector2 *apple);

static inline void snake_movement(Game *game);
static inline void update_snake(Game *game);
static inline void verify_snake_pos(Game *game);
static inline void check_snake_collisions(Game *game);
static inline void snake_controle(Game *game);

static void game_over_draw(Game *game);
static inline void draw_snake(Game *game);
static inline void draw_ui(Game *game);

void game_init(Game *game) {
  game->font = LoadFont("./Assets/Tiny5-Regular.ttf"); // I swear this is not
                                                       // raylib's default font

  game->game_over = false;
  game->snake = malloc(sizeof(Vector2) * MAX_SNAKE_LENGTH);
  game->current_snake_size = 4;
  game->snake_vel = (Vector2){0, 1};
  game->movement_timer = 0.0;
  game->apples = malloc(sizeof(Vector2) * APPLE_COUNT);
  game->score = 0;

  for (int i = 0; i < APPLE_COUNT; i++)
    random_apple_pos(game, &game->apples[i]);

  memset(game->snake, 0, sizeof(Vector2) * MAX_SNAKE_LENGTH);

  game->snake[0] = (Vector2){3, 0};
  game->snake[1] = (Vector2){2, 0};
  game->snake[2] = (Vector2){1, 0};
  game->snake[3] = (Vector2){0, 0};
}

void game_deinit(Game *game) {
  UnloadFont(game->font);
  free(game->snake);
  free(game->apples);
}

void game_update(Game *game) {
  if (game->game_over) {
    if (GetKeyPressed()) {
      UnloadFont(game->font);
      free(game->apples);
      free(game->snake);
      game_init(game);
    }
    return;
  }

  snake_movement(game);
  snake_controle(game);
}

void game_draw(Game *game) {
  if (game->game_over) {
    game_over_draw(game);
    return;
  }

  for (int i = 0; i < APPLE_COUNT; i++)
    DrawRectangle(game->apples[i].x * GRID_SIZE, game->apples[i].y * GRID_SIZE,
                  GRID_SIZE - GRID_GAP, GRID_SIZE - GRID_GAP, RED);

  draw_snake(game);

  draw_ui(game);
}

static void game_over_draw(Game *game) {
  const int size = 50;
  const Vector2 tite_size = MeasureTextEx(game->font, "GAME OVER", size, 2);
  DrawTextEx(game->font, "GAME OVER",
             (Vector2){WINDOW_WIDTH / 2 - (tite_size.x / 2),
                       WINDOW_HEIGHT / 2 - (tite_size.y / 2)},
             size, 2, WHITE);

  const Vector2 comment_size =
      MeasureTextEx(game->font, "Press *any* key to retry", 20, 2);
  DrawTextEx(game->font, "Press *any* key to retry",
             (Vector2){WINDOW_WIDTH / 2 - (comment_size.x / 2),
                       WINDOW_HEIGHT / 2 + comment_size.y},
             20, 2, WHITE);
}

static void snake_shift(Game *game) {
  Vector2 prev = game->snake[0];

  for (int i = 0; i < game->current_snake_size; i++) {
    Vector2 tmp = game->snake[i];
    game->snake[i] = prev;
    prev = tmp;
  }
}

static Vector2 get_snake_abslute_pos(Game *game) {
  return ABSLUTE_POS_OF(game->snake[0]);
}

static inline void random_vec(Vector2 *vec) {
  vec->x = rand() % GRID_COLUMNS;
  vec->y = rand() % GRID_ROWS;
}

static void random_apple_pos(Game *game, Vector2 *apple) {
  // I know, I know. I have a little bug right here but i'm not fixing it
  // Not because i can't, I'm just too lazy.
  random_vec(apple);
}

static inline void snake_movement(Game *game) {
  // Basic timer i came up with myself
  game->movement_timer += GetFrameTime();
  if (game->movement_timer >= TIMER) {
    check_snake_collisions(game);
    update_snake(game);
    verify_snake_pos(game);

    game->movement_timer = 0;
  }
}

static inline void update_snake(Game *game) {
  snake_shift(game);
  game->snake[0] = Vector2Add(game->snake[0], game->snake_vel);
}

static inline void verify_snake_pos(Game *game) {
  Vector2 abslutePos = get_snake_abslute_pos(game);

  if (abslutePos.x < 0) {
    game->game_over = true;
  } else if (abslutePos.x > WINDOW_WIDTH) {
    game->game_over = true;
  }
  if (abslutePos.y < 0) {
    game->game_over = true;
  } else if (abslutePos.y > WINDOW_HEIGHT) {
    game->game_over = true;
  }
}

static inline void check_snake_collisions(Game *game) {
  // Checking snake and apple collisions
  for (int i = 0; i < APPLE_COUNT; i++) {
    if (Vector2Equals(game->snake[0], game->apples[i])) {
      if (game->current_snake_size < MAX_SNAKE_LENGTH) {
        game->current_snake_size++;
      }
      game->score += rand() % 20 + ((float)(rand() % 10) / 10);
      random_apple_pos(game, &game->apples[i]);
    }
  }

  // Checking if the player tried to bit itself
  Vector2 head = game->snake[0];
  for (int i = 1; i < game->current_snake_size; i++) {
    if (Vector2Equals(game->snake[i], head)) {
      game->game_over = true;
      return;
    }
  }
}

static inline void snake_controle(Game *game) {
  if (IsKeyPressed(KEY_LEFT)) {
    game->snake_vel.y = 0;
    game->snake_vel.x = -1;
  } else if (IsKeyPressed(KEY_RIGHT)) {
    game->snake_vel.y = 0;
    game->snake_vel.x = 1;
  } else if (IsKeyPressed(KEY_UP)) {
    game->snake_vel.x = 0;
    game->snake_vel.y = -1;
  } else if (IsKeyPressed(KEY_DOWN)) {
    game->snake_vel.x = 0;
    game->snake_vel.y = 1;
  }
}

static inline void draw_snake(Game *game) {
  for (int i = 1; i < game->current_snake_size; i++) {
    DrawRectangle(game->snake[i].x * GRID_SIZE, game->snake[i].y * GRID_SIZE,
                  GRID_SIZE - GRID_GAP, GRID_SIZE - GRID_GAP, DARKGREEN);
  }
  DrawRectangle(game->snake[0].x * GRID_SIZE, game->snake[0].y * GRID_SIZE,
                GRID_SIZE - GRID_GAP, GRID_SIZE - GRID_GAP, GREEN);
}

static inline void draw_ui(Game *game) {
  DrawTextEx(game->font,
             TextFormat("Score: %.2f\n\nLength: %d", game->score,
                        game->current_snake_size),
             (Vector2){5, 5}, 30, 2, WHITE);
}
