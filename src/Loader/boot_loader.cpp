#include "boot_loader.h"

void *operator new(size_t size, void *buf)
{
    return buf;
}

void operator delete(void *obj, void *buf) noexcept
{
}

void operator delete(void *obj) noexcept
{
}

void BootLoader(EFI::EFI_HANDLE image_handle, EFI::EFI_SYSTEM_TABLE *system_table)
{
    gEFI = new(gEFI_buf) EFI(system_table);
    gEFI->initEFI();
    LoadKernel(image_handle);
}