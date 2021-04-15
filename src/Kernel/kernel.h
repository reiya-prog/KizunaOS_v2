#pragma once

#include <stdint.h>
#include <stddef.h>

enum PixelFormat
{
    kPixelRGB8BitPerColor,
    kPixelBGR8BitPerColor,
};

typedef struct FrameBuffer
{
    uint8_t *frame_buffer_base;
    uint32_t pixels_per_scan_line;
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    enum PixelFormat pixel_format;
}FrameBuffer;

extern "C" void __cxa_pure_virtual();
extern "C" void kernel_start(const FrameBuffer& frame_buffer);
void kernel(const FrameBuffer &frame_buffer);