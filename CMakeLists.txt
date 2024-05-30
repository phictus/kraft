cmake_minimum_required(VERSION 3.20)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(CGLM_STATIC ON)

add_subdirectory("lib/glfw")
add_subdirectory("lib/glad")
add_subdirectory("lib/cglm")
add_subdirectory("lib/stb")

project("kraft" LANGUAGES C)
add_executable("kraft")

target_sources(
    "kraft"
    PRIVATE
    "src/main.c"
    "src/game.h"
    "src/game.c"
    "src/window.h"
    "src/window.c"
    "src/renderer.h"
    "src/renderer.c"
    "src/camera.h"
    "src/camera.c"
    "src/util/type.h"
    "src/util/debug.h"
)

target_include_directories(
    "kraft"
    PRIVATE
    "lib/glfw/include"
    "lib/glad/include"
    "lib/cglm/include"
    "lib/stb/include"
)

target_link_libraries(
    "kraft"
    PRIVATE
    "glfw"
    "glad"
    "cglm"
    "stb"
    "opengl32"
)
