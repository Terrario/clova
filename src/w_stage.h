#ifndef w_stage_h
#define w_stage_h

#include <stdbool.h>

void w_stage_init(void);
void w_stage_update(void);
void w_stage_draw(void);

bool w_stage_key_matches_sentence(int key);
int  w_stage_key_current(void);
void w_stage_buff_push(int key);

#endif

