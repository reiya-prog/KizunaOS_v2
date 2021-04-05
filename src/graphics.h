#pragma once

#include "kernel.h"

typedef struct
{
    uint8_t red, green, blue;
} PixelColor;

class PixelWriter
{
public:
    PixelWriter(const FrameBuffer &frame_buffer) : frame_buffer_{frame_buffer}
    {
    }
    virtual ~PixelWriter() = default;
    virtual void Write(int x, int y, const PixelColor &color) = 0;

protected:
    uint8_t *PixelAt(int x, int y)
    {
        return frame_buffer_.frame_buffer_base + 4 * (frame_buffer_.pixels_per_scan_line * y + x);
    }

private:
    const FrameBuffer &frame_buffer_;
};

class RGBPixelWriter : public PixelWriter
{
public:
    using PixelWriter::PixelWriter;

    virtual void Write(int x, int y, const PixelColor& color) override {
        auto point = PixelAt(x, y);
        point[0] = color.red;
        point[1] = color.green;
        point[2] = color.blue;
    }
};

class BGRPixelWriter : public PixelWriter
{
public:
    using PixelWriter::PixelWriter;

    virtual void Write(int x, int y, const PixelColor& color) override {
        auto point = PixelAt(x, y);
        point[0] = color.blue;
        point[1] = color.green;
        point[2] = color.red;
    }
};