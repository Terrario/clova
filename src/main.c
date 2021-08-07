#include "raylib.h"

#include "globals.h"
#include "w_keyboard.h"
#include "w_stage.h"
#include "w_word.h"

char buff[BUFF_SIZE] = { 0 };
Font font;

int main(void)
{
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Clova");
    SetTargetFPS(60);
    font = LoadFont("assets/fonts/dejavu.fnt");

    w_keyboard_init();
    w_stage_init();
    w_word_init();

    while (!WindowShouldClose()) {
        w_keyboard_update();
        w_stage_update();
        w_word_update();

        BeginDrawing();
            ClearBackground(GRAY);

            w_keyboard_draw();
            w_stage_draw();
            w_word_draw();

            // Draw debug
            DrawFPS(0, 0);
        EndDrawing();
    }
    UnloadFont(font);
    CloseWindow();

    return 0;
}

