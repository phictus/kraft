cmake_minimum_required(VERSION 3.20)

project("stb" LANGUAGES C)
add_library("stb" STATIC)

target_sources("stb" PRIVATE "src/stb_image.c" "include/stb/stb_image.h")
target_include_directories("stb" PRIVATE "include")
