#pragma once

#include "graphics.h"
#include <stddef.h>
#include <stdarg.h>

extern const uint8_t kFontMap[256][16][8];
void WriteChar(int x, int y, char c, PixelColor color);
void WriteString(int x, int y, char *str, PixelColor color);
int printk(const char* format, ...);