#include "main.h"

char pixel_writer_buf[sizeof(RGBPixelWriter)];
PixelWriter *pixel_writer;

void RGBPixelWriter::Write(int x, int y, const PixelColor &color)
{
    auto point = PixelAt(x, y);
    point[0] = color.red;
    point[1] = color.green;
    point[2] = color.blue;
    point[3] = color.reserved;
}

void BGRPixelWriter::Write(int x, int y, const PixelColor &color)
{
    auto point = PixelAt(x, y);
    point[0] = color.blue;
    point[1] = color.green;
    point[2] = color.red;
    point[3] = color.reserved;
}