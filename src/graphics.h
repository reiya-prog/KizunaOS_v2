#pragma once

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
};

class BGRPixelWriter : public PixelWriter
{
public:
    using PixelWriter::PixelWriter;
    virtual void Write(int x, int y, const PixelColor &color) override;
};

extern char pixel_writer_buf[sizeof(RGBPixelWriter)];
extern PixelWriter *pixel_writer;