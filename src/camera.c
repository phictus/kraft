#include <math.h>

#include "util/debug.h"
#include "window.h"
#include "camera.h"

#define MOVEMENT_SPEED 30.0f

static CameraSpec camera_spec;
static f32 yaw;
static f32 pitch;
static vec3 position;
static vec3 direction;
static mat4 projection;
static mat4 view;
static mat4 view_projection;

static void read_camera_spec() {
    FILE* stream;
    ASSERT(fopen_s(&stream, "assets/camera_spec.ini", "r") == 0, "Reading assets/camera_spec.ini failed!\n");

    fscanf_s(stream, "%f", &camera_spec.fovy);
    fscanf_s(stream, "%f", &camera_spec.znear);
    fscanf_s(stream, "%f", &camera_spec.zfar);
    fscanf_s(stream, "%f", &camera_spec.xsensitivity);
    fscanf_s(stream, "%f", &camera_spec.ysensitivity);

    fclose(stream);
}

void camera_init() {
    read_camera_spec();

    const WindowSpec* window_spec = window_get_spec();
    glm_perspective(
        glm_rad(camera_spec.fovy),
        (f32)window_spec->width / (f32)window_spec->height,
        camera_spec.znear,
        camera_spec.zfar,
        projection
    );

    glm_vec3_zero(position);
    yaw = 0.0f;
    pitch = 0.0f;
}

void camera_update() {
    f32 dt = window_get_dt();
    const vec2* delta_mouse = window_get_delta_mouse();
    yaw += (*delta_mouse)[0] * camera_spec.xsensitivity;
    pitch += (*delta_mouse)[1] * camera_spec.ysensitivity;
    pitch = glm_clamp(pitch, -89.99f, 89.99f);

    const f32 cos_pitch = cosf(glm_rad(pitch));
    direction[0] = cosf(glm_rad(yaw)) * cos_pitch;
    direction[1] = sinf(glm_rad(pitch));
    direction[2] = sinf(glm_rad(yaw)) * cos_pitch;

    vec3 right;
    glm_cross(direction, GLM_YUP, right);
    glm_normalize(right);

    vec3 velocity = GLM_VEC3_ZERO_INIT;
    if (window_is_key_pressed(KEY_W)) {
        glm_vec3_add(velocity, direction, velocity);
    }
    if (window_is_key_pressed(KEY_S)) {
        glm_vec3_sub(velocity, direction, velocity);
    }
    if (window_is_key_pressed(KEY_D)) {
        glm_vec3_add(velocity, right, velocity);
    }
    if (window_is_key_pressed(KEY_A)) {
        glm_vec3_sub(velocity, right, velocity);
    }
    glm_vec3_scale(velocity, MOVEMENT_SPEED * dt, velocity);
    glm_vec3_add(position, velocity, position);

    glm_look(position, direction, GLM_YUP, view);
    glm_mat4_mul(projection, view, view_projection);
}

const CameraSpec* camera_get_spec() {
    return &camera_spec;
}

const vec3* camera_get_position() {
    return &position;
}

const vec3* camera_get_direction() {
    return &direction;
}

const mat4* camera_get_view_projection() {
    return &view_projection;
}
