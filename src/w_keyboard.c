#include "raylib.h"
#include <stdint.h>

#include "globals.h"
#include "w_keyboard.h"
#include "w_stage.h"

#define KEY_COUNT    32
#define FRAME_WIDTH  14
#define FRAME_HEIGHT 14

Texture2D tex;
Rectangle spr[KEY_COUNT * 3] = { 0 };

enum KeyAnimationType {
    ANIM_NORMAL = 0
    , ANIM_NEGATIVE
};

uint8_t anim_type[KEY_COUNT] = { ANIM_NORMAL };
uint8_t anim_phase[KEY_COUNT] = { 0 };
float anim_acc[KEY_COUNT] = { 0.0f };

int _get_key_spr_index(int key);

void w_keyboard_init(void)
{
    uint8_t i;
    tex = LoadTexture("assets/keyboard.png");

    for (i = 0; i < KEY_COUNT; ++i) {
        spr[i] = (Rectangle) {
            0,
            FRAME_HEIGHT * i,
            FRAME_WIDTH,
            FRAME_HEIGHT
        };
        spr[i + KEY_COUNT] = (Rectangle) {
            FRAME_WIDTH,
            FRAME_HEIGHT * i,
            FRAME_WIDTH,
            FRAME_HEIGHT
        };
        spr[i + KEY_COUNT * 2] = (Rectangle) {
            FRAME_WIDTH * 2,
            FRAME_HEIGHT * i,
            FRAME_WIDTH,
            FRAME_HEIGHT
        };
    }
}

void w_keyboard_update(void)
{
    bool match;
    uint8_t i, key_index;
    int key;

    key = GetCharPressed();
    if (key > 0) {
        match = w_stage_key_matches_sentence(key);
        if (match) {
            w_stage_buff_push(key);
        }

        key_index = _get_key_spr_index(key);
        if (key_index >= 0 && key_index < KEY_COUNT) {
            anim_acc[key_index] = 0.0f;
            anim_phase[key_index] = 1;
            anim_type[key_index] = match ? ANIM_NORMAL : ANIM_NEGATIVE;
        }
    }

    for (i = 0; i < KEY_COUNT; ++i) {
        if (anim_phase[i] == 2) {
            anim_phase[i] = 0;
        } else if (anim_phase[i] > 0) {
            if (anim_acc[i] > 0.02f / 3) {
                anim_acc[i] = 0.0f;
                anim_phase[i]++;
            } else {
                anim_acc[i] += GetFrameTime();
            }
        }
    }
}

void w_keyboard_draw(void)
{
    Rectangle dest;
    Color tint;
    uint8_t
        i
        , r1 = 12
        , r2 = 11
        , r3 = 9
        , spacing_x = 5
        , spacing_y = 4;
    uint16_t
        x
        , y = 0
        , rw
        , offset_y = (GetScreenHeight() / 2) + (FRAME_HEIGHT * SCALE);
    int highlight_key = w_stage_key_current();

    y = 0;
    rw = FRAME_WIDTH * r1 * SCALE + (r1 * spacing_x);
    for (i = 0; i < r1; ++i) {
        x = FRAME_WIDTH * i;
        x = ((GetScreenWidth() - rw) / 2) + (x * SCALE);

        dest = (Rectangle) {
            x + (spacing_x * i),
            offset_y + y * SCALE,
            FRAME_WIDTH * SCALE,
            FRAME_HEIGHT * SCALE
        };

        tint = WHITE;
        if (anim_phase[i] > 0) {
            tint = GREEN;

            if (anim_type[i] == ANIM_NEGATIVE) {
                tint = PINK;
            }
        }

        if (hint_mode && _get_key_spr_index(highlight_key) == i) {
            // TODO: use a crosshair instead
            DrawRectangleLinesEx(dest, 2, YELLOW);
        }

        DrawTexturePro(
            tex,
            spr[i + KEY_COUNT * anim_phase[i]],
            dest,
            (Vector2) { 0, 0 },
            0.0f,
            tint
        );
    }

    y = FRAME_HEIGHT + spacing_y;
    rw = FRAME_WIDTH * r2 * SCALE + (r2 * spacing_x);
    for (i = 1; i < r2 + 1; ++i) {
        x = FRAME_WIDTH * (i - 1);
        x = ((GetScreenWidth() - rw) / 2) + (x * SCALE);

        dest = (Rectangle) {
            x + (spacing_x * i),
            offset_y + y * SCALE,
            FRAME_WIDTH * SCALE,
            FRAME_HEIGHT * SCALE
        };

        tint = WHITE;
        if (anim_phase[i + r2] > 0) {
            tint = GREEN;

            if (anim_type[i + r2] == ANIM_NEGATIVE) {
                tint = PINK;
            }
        }

        if (hint_mode && _get_key_spr_index(highlight_key) == i + r2) {
            // TODO: use a crosshair instead
            DrawRectangleLinesEx(dest, 2, YELLOW);
        }

        DrawTexturePro(
            tex,
            spr[i + r2 + KEY_COUNT * anim_phase[i + r2]],
            dest,
            (Vector2) { 0, 0 },
            0.0f,
            tint
        );
    }

    y = FRAME_HEIGHT * 2 + spacing_y * 2;
    rw = FRAME_WIDTH * r3 * SCALE + (r3 * spacing_x);
    for (i = 0; i < r3; ++i) {
        x = FRAME_WIDTH  * i;
        x = ((GetScreenWidth() - rw) / 2) + (x * SCALE);

        dest = (Rectangle) {
            x + (spacing_x * i),
            offset_y + y * SCALE,
            FRAME_WIDTH * SCALE,
            FRAME_HEIGHT * SCALE
        };

        tint = WHITE;
        if (anim_phase[i + r1 + r2] > 0) {
            tint = GREEN;

            if (anim_type[i + r1 + r2] == ANIM_NEGATIVE) {
                tint = PINK;
            }
        }

        if (hint_mode && _get_key_spr_index(highlight_key) == i + r1 + r2) {
            // TODO: use a crosshair instead
            DrawRectangleLinesEx(dest, 2, YELLOW);
        }

        DrawTexturePro(
            tex,
            spr[i + r1 + r2 + KEY_COUNT * anim_phase[i + r1 + r2]],
            dest,
            (Vector2) { 0, 0 },
            0.0f,
            tint
        );
    }
}

int _get_key_spr_index(int unicode_key)
{
    switch (unicode_key) {
        // ?? ??
        case 0x0419: case 0x0439:
            return 0;
        // ?? ??
        case 0x0426: case 0x0446:
            return 1;
        // ?? ??
        case 0x0423: case 0x0443:
            return 2;
        // ?? ??
        case 0x041a: case 0x043a:
            return 3;
        // ?? ??
        case 0x0415: case 0x0435:
            return 4;
        // ?? ??
        case 0x043d: case 0x041d:
            return 5;
        // ?? ??
        case 0x0413: case 0x0433:
            return 6;
        // ?? ??
        case 0x0448: case 0x0428:
            return 7;
        // ?? ??
        case 0x0429: case 0x0449:
            return 8;
        // ?? ??
        case 0x0417: case 0x0437:
            return 9;
        // ?? ??
        case 0x0445: case 0x0425:
            return 10;
        // ?? ??
        case 0x042a: case 0x044a:
            return 11;
        // ?? ??
        case 0x0444: case 0x0424:
            return 12;
        // ?? ??
        case 0x044b: case 0x042b:
            return 13;
        // ?? ??
        case 0x0432: case 0x0412:
            return 14;
        // ?? ??
        case 0x0410: case 0x0430:
            return 15;
        // ?? ??
        case 0x043f: case 0x041f:
            return 16;
        // ?? ??
        case 0x0420: case 0x0440:
            return 17;
        // ?? ??
        case 0x041e: case 0x043e:
            return 18;
        // ?? ??
        case 0x041b: case 0x043b:
            return 19;
        // ?? ??
        case 0x0434: case 0x0414:
            return 20;
        // ?? ??
        case 0x0416: case 0x0436:
            return 21;
        // ?? ??
        case 0x044d: case 0x042d:
            return 22;
        // ?? ??
        case 0x042f: case 0x044f:
            return 23;
        // ?? ??
        case 0x0427: case 0x0447:
            return 24;
        // ?? ??
        case 0x0441: case 0x0421:
            return 25;
        // ?? ??
        case 0x043c: case 0x041c:
            return 26;
        // ?? ??
        case 0x0418: case 0x0438:
            return 27;
        // ?? ??
        case 0x0442: case 0x0422:
            return 28;
        // ?? ??
        case 0x042c: case 0x044c:
            return 29;
        case 0x0431: case 0x0411:
            return 30;
        // ?? ??
        case 0x044e: case 0x042e:
            return 31;
        // ?? ??
        case 0x0451: case 0x0401:
            TraceLog(LOG_ERROR, "Draw the freaking ?? ?? sprite!");
            return -1;
        default:
            return -1;
    }
}

