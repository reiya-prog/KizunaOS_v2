#pragma once

#include <cstdint>
#include "elf.h"
#include "efi.h"
#include "graphics.h"

void LoadKernel(EFI::EFI_HANDLE ImageHandle, EFI *efi, FrameBuffer *fb);
// void puts(EFI *efi, unsigned short *s);
// void puth(EFI *efi, unsigned long long val, unsigned char num_digits);
// int strcmp(const char *s1, const char *s2);
// void memset(void *dst, int c, int n);
extern "C" void memcpy(void *dst, const void *src, int n);