#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

#define min(a, b) a > b ? b : a
#define max(a, b) a > b ? a : b

void *operator new(size_t size, void *buf);
void operator delete(void *obj, void *buf);
void operator delete(void *obj);
void memset(void *dst, int value, int size);
extern "C" void memcpy(void *dst, const void *src, int size);