#ifndef UTIL_DEBUG_H
#define UTIL_DEBUG_H

#include <stdio.h>
#include <stdlib.h>

#define LOG_INFO(msg, ...) fprintf(stdout, msg, __VA_ARGS__)
#define LOG_ERROR(msg, ...) fprintf(stderr, msg, __VA_ARGS__)

#define ASSERT(exp, msg, ...) if (!(exp)) { LOG_ERROR(msg, __VA_ARGS__); exit(1); }

#endif
