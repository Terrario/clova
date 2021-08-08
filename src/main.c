#include "raylib.h"

#include "globals.h"
#include "w_keyboard.h"
#include "w_stage.h"

bool hint_mode = true;
char buff[BUFF_SIZE] = { 0 };
Font font;

int main(void)
{
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Clova");
    SetTargetFPS(60);
    InitAudioDevice();

    font = LoadFont("assets/fonts/dejavu.fnt");

    w_keyboard_init();
    w_stage_init();

    while (!WindowShouldClose()) {
        w_keyboard_update();
        w_stage_update();

        BeginDrawing();
            ClearBackground(GRAY);

            w_keyboard_draw();
            w_stage_draw();

            // Draw debug
            DrawFPS(0, 0);
        EndDrawing();
    }
    UnloadFont(font);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

