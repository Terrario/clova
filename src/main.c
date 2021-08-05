#include "raylib.h"

int main(void)
{
    InitWindow(800, 600, "Clova");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(WHITE);
            DrawFPS(0, 0);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}

