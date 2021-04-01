#pragma once

#include "main.h"
#include "efi.h"
#include "elfloader.h"

void BootLoader(EFI::EFI_HANDLE image_handle, EFI::EFI_SYSTEM_TABLE *system_table);