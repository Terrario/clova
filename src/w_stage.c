#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "globals.h"
#include "w_stage.h"

char *sentence;
uint16_t buff_cursor = 0;

Sound
    typing_positive_feedback
    , typing_negative_feedback
    , positive_feedback
    , sentence_audio = { 0 };

void _next_random_sentence(void);

void w_stage_init(void)
{
    _next_random_sentence();

    typing_positive_feedback = LoadSound("assets/sfx/electric/whoosh.wav");
    typing_negative_feedback = LoadSound("assets/sfx/electric/uielec01.wav");
    positive_feedback = LoadSound("assets/sfx/electric/enginestart.wav");
}

void w_stage_update(void)
{
    int
        i
        , buff_len
        , sentence_len;

    // replay sentence audio
    if (IsKeyDown(KEY_LEFT_SUPER)) {
        if (IsKeyPressed(KEY_P)) {
            PlaySound(sentence_audio);
        }

        if (IsKeyPressed(KEY_N)) {
            _next_random_sentence();
        }
    }

    GetCodepoints(buff, &buff_len);
    GetCodepoints(sentence, &sentence_len);

    // Strings aren't same size, exit
    if (buff_len != sentence_len) {
        return;
    }

    _next_random_sentence();
    for (i = 0; i < buff_cursor; ++i) {
        buff[i] = 0;
    }
    buff_cursor = 0;

    PlaySound(positive_feedback);
}

void w_stage_draw(void)
{
   float 
        font_size = 15.0f * SCALE
        , spacing = 1.0f * SCALE
        , offset_x
        , offset_y;
    uint8_t i = 15;
    Vector2 sentence_size;

    sentence_size = MeasureTextEx(font, sentence, font_size, spacing);
    while (sentence_size.x > GetScreenWidth() * .8f) {
        font_size = i-- * SCALE;
        sentence_size = MeasureTextEx(font, sentence, font_size, spacing);
    }

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

int w_stage_key_current(void)
{
    int
        sentence_key
        , sentence_char_length;

    return GetNextCodepoint(&sentence[buff_cursor], &sentence_char_length);
}

bool w_stage_key_matches_sentence(int key)
{
    int sentence_key = w_stage_key_current();

    if (sentence_key == key) {
        PlaySound(typing_positive_feedback);
        return true;
    }

    PlaySound(typing_negative_feedback);
    return false;
}

void w_stage_buff_push(int key)
{
    const char *unicode_char;
    uint16_t i;
    int len;

    unicode_char = CodepointToUtf8(key, &len);
    for (i = 0; i < len; ++i) {
        buff[buff_cursor++] = unicode_char[i];
    }
}

void w_stage_buff_pop(void)
{
    if (buff_cursor == 0) {
        return;
    }

    uint16_t i;
    int
        *codepoints
        , codepoint_count
        , len;

    codepoints = GetCodepoints(buff, &codepoint_count);
    CodepointToUtf8(codepoints[codepoint_count -1], &len);

    for (i = 0; i < len; ++i) {
        buff[--buff_cursor] = 0;
    }
}

int _random_sentence_callback(void *data, int argc, char **argv, char **col_name)
{
    uint16_t str_size = 0;
    char *filename;

    int i
        , *codepoints
        , codepoints_count
        , bytes_length;

    if (sentence_audio.stream.buffer != NULL) {
        UnloadSound(sentence_audio);
    }

    filename = malloc(strlen("assets/audio/") + strlen(argv[0]) + strlen(".mp3"));
    sprintf(filename, "assets/audio/%s.mp3", argv[0]);
    sentence_audio = LoadSound(filename);
    free(filename);

    codepoints = GetCodepoints(argv[1], &codepoints_count);
    for (i = 0; i < codepoints_count; ++i) {
        CodepointToUtf8(codepoints[i], &bytes_length);
        str_size += bytes_length;
    }

    sentence = realloc(sentence, str_size + 1);
    strcpy(sentence, argv[1]);

    return 0;
}

void _next_random_sentence(void)
{
    const char *sql = "select t.sentence_id, t.content, t.translated_content from translation t inner join sentence_audio sa on sa.sentence_id = t.sentence_id where sa.license is not null order by random() limit 1;";
    int rc;
    char *error_message = 0;
    const char *data = "";

    rc = sqlite3_exec(db, sql, _random_sentence_callback, (void*) data, &error_message);
    if (rc != SQLITE_OK) {
        sqlite3_free(error_message);
        TraceLog(LOG_ERROR, "DB Error: Could not get next random sentence.");
    }

    PlaySound(sentence_audio);
}

