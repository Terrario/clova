#ifndef globals_h
#define globals_h

#include "raylib.h"

#define GAME_WIDTH  800 * 1.5
#define GAME_HEIGHT 600 * 1.5
#define SCALE       GAME_WIDTH * 0.0055f
#define BUFF_SIZE   1024

extern const char *sentence;
extern char buff[BUFF_SIZE];
extern Font font;

#endif

