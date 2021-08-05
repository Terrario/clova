#include "raylib.h"

#include "w_keyboard.h"

#define GAME_WIDTH  800 * 1.5
#define GAME_HEIGHT 600 * 1.5

int main(void)
{
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Clova");
    SetTargetFPS(60);

    w_keyboard_init();

    while (!WindowShouldClose()) {
        w_keyboard_update();

        BeginDrawing();
            ClearBackground(GRAY);

            w_keyboard_draw();

            // Draw debug
            DrawFPS(0, 0);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}

