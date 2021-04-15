#pragma once

#include "kernel.h"
#include <stdint.h>
#include <stddef.h>

typedef struct PixelColor
{
    uint8_t red, green, blue, reserved;
} PixelColor;

class PixelWriter
{
public:
    PixelWriter(const FrameBuffer &frame_buffer) : frame_buffer_{frame_buffer}
    {
    }
    virtual ~PixelWriter() = default;
    virtual void Write(int x, int y, const PixelColor &color) = 0;
    virtual void Write(int x, int y, const uint64_t pallet) = 0;

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
    virtual void Write(int x, int y, const PixelColor &color) override;
    virtual void Write(int x, int y, const uint64_t pallet) override;
};

class BGRPixelWriter : public PixelWriter
{
public:
    using PixelWriter::PixelWriter;
    virtual void Write(int x, int y, const PixelColor &color) override;
    virtual void Write(int x, int y, const uint64_t pallet) override;
};

PixelColor ConvertColor(uint64_t pallet);
extern char pixel_writer_buf[sizeof(RGBPixelWriter)];
extern PixelWriter *pixel_writer;