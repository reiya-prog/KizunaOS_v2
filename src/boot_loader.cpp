#include "main.h"

void *operator new(size_t size, void *buf)
{
    return buf;
}

void operator delete(void *obj, void *buf) noexcept
{
}

FrameBuffer *InitFrameBuffer(EFI::EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP)
{
    FrameBuffer *fb = nullptr;

    fb->frame_buffer_base = reinterpret_cast<unsigned long long *>(GOP->Mode->FrameBufferBase);
    fb->frame_buffer_size = static_cast<unsigned long long>(GOP->Mode->FrameBufferSize);
    fb->resolution_H = static_cast<unsigned int>(GOP->Mode->Info->HorizontalResolution);
    fb->resolution_V = static_cast<unsigned int>(GOP->Mode->Info->VerticalResolution);

    return fb;
}

void BootLoader(EFI::EFI_HANDLE image_handle, EFI::EFI_SYSTEM_TABLE *system_table)
{
    gEFI = new(gEFI_buf) EFI(system_table);
    gEFI->initEFI();
    FrameBuffer *fb = InitFrameBuffer(gEFI->getGraphicsOutputProtocol());
    LoadKernel(image_handle, fb);
}