#pragma once

#include "efi.h"
#include "kernel.h"

extern "C"{
    __attribute__((ms_abi)) void io_hlt(void);
}