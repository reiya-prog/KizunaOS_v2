#pragma once

#include <stdint.h>
#include "elf.h"
#include "efi.h"
#include "kernel.h"
#include "std_func.h"

void LoadKernel(EFI::EFI_HANDLE ImageHandle);