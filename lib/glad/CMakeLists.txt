cmake_minimum_required(VERSION 3.20)

project("glad" LANGUAGES C)
add_library("glad" STATIC)

target_sources("glad" PRIVATE "src/gl.c" "include/KHR/khrplatform.h" "include/glad/gl.h")
target_include_directories("glad" PRIVATE "include")
