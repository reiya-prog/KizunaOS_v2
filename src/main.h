#pragma once

#include "efi.h"
#include "boot_loader.h"
#include "elf.h"
#include "efi_kernel_loader.h"
#include "loader_asm.h"
#include "kernel_asm.h"
#include "kernel.h"
#include "graphics.h"
#include "std_func.h"
#include "font.h"
#include "console.h"
#include "descriptor.h"
#include "libc_support.h"
// #include "handler.h"
// #include "keyboard.h"
// #include "pic.h"
// #include "stl.h"
// #include "global.h"
#include <cstdio>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>