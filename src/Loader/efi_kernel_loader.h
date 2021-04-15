#pragma once

#include <stddef.h>
#include <stdint.h>
#include "elf.h"
#include "efi.h"
#include "kernel.h"
#include "loader_asm.h"

void LoadKernel(EFI::EFI_HANDLE ImageHandle);