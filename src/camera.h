#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

#include "util/type.h"

typedef struct {
    f32 fovy;
    f32 znear;
    f32 zfar;
    f32 xsensitivity;
    f32 ysensitivity;
} CameraSpec;

void camera_init();
void camera_update();

const CameraSpec* camera_get_spec();
const vec3* camera_get_position();
const vec3* camera_get_direction();
const mat4* camera_get_view_projection();

#endif
