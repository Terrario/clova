#include "raylib.h"
#include <sqlite3.h>

#include "globals.h"
#include "w_keyboard.h"
#include "w_stage.h"

bool hint_mode = true;
char buff[BUFF_SIZE] = { 0 };
Font font;
sqlite3 *db;

int main(void)
{
    int db_rc;
    sqlite3_open("assets/db.sqlite3", &db);
    if (sqlite3_open("assets/db.sqlite3", &db)) {
        sqlite3_close(db);
        TraceLog(LOG_ERROR, "Could not open sqlite3 db: assets/db.sqlite3");
    }

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
    sqlite3_close(db);

    return 0;
}

