#pragma once

#include "efi.h"
#include "kernel.h"

extern "C"{
    __attribute__((ms_abi)) void io_hlt(void);
    __attribute__((ms_abi)) void io_out32(uint16_t config_addr, uint32_t addr);
    __attribute__((ms_abi)) uint32_t io_in32(uint16_t config_addr);
}