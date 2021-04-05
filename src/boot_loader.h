#pragma once

#include "efi.h"
#include "efi_kernel_loader.h"
#include "kernel.h"

void BootLoader(EFI::EFI_HANDLE image_handle, EFI::EFI_SYSTEM_TABLE *system_table);