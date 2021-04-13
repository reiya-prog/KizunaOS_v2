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
//void memcpy(void *dst, const void *src, int size);

int dec_to_asc(char *str, int dec);
int hex_to_asc(char *str, int dec);
void sprintf(char *str, char *format, ...);