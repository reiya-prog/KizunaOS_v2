#include "main.h"

extern "C" void __cxa_pure_virtual() { io_hlt(); }

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
    }
    for (int y = 0; y < frame_buffer.vertical_resolution; ++y)
    {
        for (int x = 0; x < frame_buffer.horizontal_resolution; ++x)
        {
            pixel_writer->Write(x, y, {255, 255, 255});
        }
    }
    console = new (console_buf) Console{*pixel_writer, {0, 0, 0}, {255, 255, 255}};

    for (int i = 0; i < frame_buffer.horizontal_resolution; ++i)
    {
        for (int j = 0; j < frame_buffer.vertical_resolution; ++j)
        {
            pixel_writer->Write(i, j, 0x20b2aaLL);
        }
    }
    printk("Linking to you, access our connection!\n\tKizunaOS, boot up!\n");

    io_hlt();
}