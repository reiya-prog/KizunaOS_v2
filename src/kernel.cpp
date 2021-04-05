#include "main.h"

extern "C"
{
    void kernel_start(const FrameBuffer& frame_buffer)
    {
        for (unsigned long long fb_i = 0; fb_i < frame_buffer.frame_buffer_size; ++fb_i)
        {
            frame_buffer.frame_buffer_base[fb_i] = fb_i % 256;
        }
        io_hlt();
    }
}