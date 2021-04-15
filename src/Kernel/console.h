#pragma once

#include "graphics.h"
#include <stdarg.h>
#include <cstring>

class Console
{
public:
    static const int kRows = 25, kColumns = 80;

    Console(PixelWriter &pixel_writer, const PixelColor &fore_color, const PixelColor &back_color);
    ~Console() = default;

    void PrintString(const char *str);

private:
    void NewLine();
    PixelWriter &pixel_writer_;
    const PixelColor fore_color_, back_color_;
    char strings_[kRows][kColumns + 1];
    int cursor_row_, cursor_column_;
};

void WriteChar(int x, int y, char c, PixelColor color);
void WriteString(int x, int y, char *str, PixelColor color);
int printk(const char* format, ...);
extern char console_buf[sizeof(Console)];
extern Console *console;