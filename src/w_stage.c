#include "raylib.h"
#include <string.h>
#include <stdint.h>

#include "globals.h"
#include "w_stage.h"
#include "w_word.h"

uint16_t buff_cursor = 0;

void w_stage_init(void)
{
}

void w_stage_buff_push(int key)
{
    const char *unicode_char;
    int len;

    // Convert key into codepoint
    unicode_char = CodepointToUtf8(key, &len);

    // Append to buffer
    strcpy(&buff[buff_cursor], unicode_char);

    // Move buffer cursor
    buff_cursor += len;
}

void w_stage_buff_pop(void)
{
    uint8_t chars;
    uint16_t i;
    int len;

    if (buff_cursor == 0) {
        return;
    }

    chars = GetCodepointsCount(buff);
    for (i = 0; i < chars; ++i) {
        GetNextCodepoint(buff, &len);
    }

    buff_cursor -= len;
    for (i = 0; i < len; ++i) {
        buff[buff_cursor + i] = 0;
    }
}

void w_stage_update(void)
{
}

void w_stage_draw(void)
{
   float 
        font_size = 15.0f * SCALE
        , spacing = 1.0f * SCALE
        , offset_x
        , offset_y;
    Vector2 sentence_size;

    sentence_size = MeasureTextEx(font, sentence, font_size, spacing);

    offset_x = GetScreenWidth() / 2 - sentence_size.x / 2;
    offset_y = GetScreenHeight() / 2 - sentence_size.y / 2;

    DrawTextEx(
        font,
        sentence,
        (Vector2) { offset_x, offset_y },
        font_size,
        spacing,
        ColorAlpha(WHITE, .7f)
    );

    DrawTextEx(
        font,
        buff,
        (Vector2) { offset_x, offset_y },
        font_size,
        spacing,
        WHITE
    );

    DrawRectangle(
        offset_x,
        offset_y + sentence_size.y,
        sentence_size.x,
        1 * SCALE,
        SKYBLUE
    );
    DrawRectangle(
        offset_x + (sentence_size.x * 0.9f / 16),
        offset_y + sentence_size.y + 1 * SCALE,
        sentence_size.x * 0.9f,
        1 * SCALE,
        BLUE
    );
    DrawRectangle(
        offset_x + (sentence_size.x * 0.8f / 8),
        offset_y + sentence_size.y + 2 * SCALE - 1,
        sentence_size.x * 0.8f,
        1 * SCALE,
        DARKBLUE
    );
}

