#include "Game.h"
#include <raylib.h>
#include <stdlib.h>
#include <time.h>

int main() {
  srand(time(NULL));
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "C Snake");
  SetTargetFPS(60);

  Game game;
  game_init(&game);

  while (!WindowShouldClose()) {
    game_update(&game);

    BeginDrawing();
    ClearBackground(BLACK);
    game_draw(&game);
    EndDrawing();
  }

  game_deinit(&game);
  CloseWindow();
  return 0;
}
