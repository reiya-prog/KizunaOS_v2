#pragma once

#include <stdarg.h>

#define min(a, b) a > b ? b : a
#define max(a, b) a > b ? a : b

void memset(void *dst, int value, int size);

extern "C" void memcpy(void *dst, const void *src, int size);