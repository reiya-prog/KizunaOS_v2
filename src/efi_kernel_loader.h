#pragma once

#include <cstdint>
#include "elf.h"
#include "efi.h"
#include "kernel.h"
#include "std_func.h"

void LoadKernel(EFI::EFI_HANDLE ImageHandle, FrameBuffer *fb);