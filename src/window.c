#include <GLFW/glfw3.h>

#include "util/debug.h"
#include "window.h"

static GLFWwindow* window = NULL;
static WindowSpec window_spec;
static f64 last_frame_time = 0;
static f32 dt = 0;
static vec2 delta_mouse = GLM_VEC2_ZERO_INIT;
static vec2 last_mouse_position = GLM_VEC2_ZERO_INIT;
static bool first_mouse = true;

static void read_window_spec() {
    FILE* stream;
    ASSERT(fopen_s(&stream, "assets/window_spec.ini", "r") == 0, "Reading assets/window_spec.ini failed!\n");

    fscanf_s(stream, "%ud", &window_spec.width);
    fscanf_s(stream, "%ud", &window_spec.height);
    fscanf_s(stream, "%ud", (u32*)&window_spec.vsync);
    fscanf_s(stream, "%ud", (u32*)&window_spec.fullscreen);

    fclose(stream);
}

void window_init() {
    read_window_spec();

    ASSERT(glfwInit(), "GLFW initialization failed!\n");

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    GLFWmonitor* monitor = NULL;
    if (window_spec.fullscreen) {
        monitor = glfwGetPrimaryMonitor();
    }

    window = glfwCreateWindow(window_spec.width, window_spec.height, "Kraft", monitor, NULL);
    ASSERT(window, "Window creation failed!\n");
    glfwMakeContextCurrent(window);

    if (window_spec.vsync) {
        glfwSwapInterval(1);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void window_deinit() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool window_is_open() {
    return !glfwWindowShouldClose(window);
}

void window_close() {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void window_poll_events() {
    glfwPollEvents();

    f64 current_frame_time = glfwGetTime();
    dt = (f32)(current_frame_time - last_frame_time);
    last_frame_time = current_frame_time;

    f64 x, y;
    glfwGetCursorPos(window, &x, &y);
    if (first_mouse) {
        last_mouse_position[0] = x;
        last_mouse_position[1] = y;
        first_mouse = false;
    }

    delta_mouse[0] = x - last_mouse_position[0];
    delta_mouse[1] = last_mouse_position[1] - y;

    last_mouse_position[0] = x;
    last_mouse_position[1] = y;
}

void window_swap_buffers() {
    glfwSwapBuffers(window);
}

bool window_is_key_pressed(Key key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

const vec2* window_get_delta_mouse() {
    return &delta_mouse;
}

f32 window_get_dt() {
    return dt;
}

const WindowSpec* window_get_spec() {
    return &window_spec;
}
