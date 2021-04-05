#pragma once

typedef struct
{
    unsigned long long *frame_buffer_base;
    unsigned long long frame_buffer_size;
    unsigned int resolution_H;
    unsigned int resolution_V;
} FrameBuffer;

typedef struct {
    FrameBuffer frame_buffer;
} BootStruct;

extern "C" void kernel_start(BootStruct* BootStruct);