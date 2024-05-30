#ifndef GAME_H
#define GAME_H

#include "util/type.h"

void game_init();
void game_deinit();

bool game_is_running();
void game_update();

#endif
