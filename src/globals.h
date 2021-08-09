#ifndef globals_h
#define globals_h

#include "raylib.h"
#include <sqlite3.h>
#include <stdbool.h>

#define GAME_WIDTH  800 * 1.5
#define GAME_HEIGHT 600 * 1.5
#define SCALE       GAME_WIDTH * 0.0055f
#define BUFF_SIZE   1024

extern bool hint_mode;
extern char *sentence;
extern char buff[BUFF_SIZE];
extern Font font;
extern sqlite3 *db;

#endif

