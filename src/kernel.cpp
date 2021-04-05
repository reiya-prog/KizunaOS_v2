#include "main.h"

void kernel(const FrameBuffer &frame_buffer);

extern "C"
{
    void kernel_start(const FrameBuffer &frame_buffer)
    {
        kernel(frame_buffer);
    }
}

void kernel(const FrameBuffer &frame_buffer)
{
    switch (frame_buffer.pixel_format)
    {
    case kPixelRGB8BitPerColor:
        pixel_writer = new (pixel_writer_buf) RGBPixelWriter{frame_buffer};
        break;
    case kPixelBGR8BitPerColor:
        pixel_writer = new (pixel_writer_buf) BGRPixelWriter{frame_buffer};
        break;
    default:
        break;
    }

    for (int x = 0; x < frame_buffer.horizontal_resolution; ++x)
    {
        for (int y = 0; y < frame_buffer.vertical_resolution; ++y)
        {
            pixel_writer->Write(x, y, {0, 255, 0});
        }
    }
    io_hlt();
}