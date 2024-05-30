#ifndef WINDOW_H
#define WINDOW_H

#include <cglm/cglm.h>

#include "util/type.h"

typedef struct {
    u32 width;
    u32 height;
    bool vsync;
    bool fullscreen;
} WindowSpec;

typedef enum {
    KEY_SPACE = 32,
    KEY_ESCAPE = 256,
    KEY_W = 87,
    KEY_A = 65,
    KEY_S = 83,
    KEY_D = 68
} Key;

void window_init();
void window_deinit();

bool window_is_open();
void window_close();

void window_poll_events();
void window_swap_buffers();

bool window_is_key_pressed(Key key);
const vec2* window_get_delta_mouse();
f32 window_get_dt();

const WindowSpec* window_get_spec();

#endif
