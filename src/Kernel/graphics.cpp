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

void RGBPixelWriter::Write(int x, int y, const uint64_t pallet)
{
    PixelColor color = ConvertColor(pallet);
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

void BGRPixelWriter::Write(int x, int y, const uint64_t pallet)
{
    PixelColor color = ConvertColor(pallet);
    auto point = PixelAt(x, y);
    point[0] = color.blue;
    point[1] = color.green;
    point[2] = color.red;
    point[3] = color.reserved;
}

PixelColor ConvertColor(const uint64_t pallet){
    // 0xffff -> 0x ff ff ff -> 11111111 11111111 11111111
    PixelColor color = {0, 0, 0, 0};
    color.red = pallet >> 16 & 0xff;
    color.green = pallet >> 8 & 0xff;
    color.blue = pallet & 0xff;
    color.reserved = 0;
    return color;
}