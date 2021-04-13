#include "boot_loader.h"

void BootLoader(EFI::EFI_HANDLE image_handle, EFI::EFI_SYSTEM_TABLE *system_table)
{
    gEFI = new(gEFI_buf) EFI(system_table);
    gEFI->initEFI();
    LoadKernel(image_handle);
}