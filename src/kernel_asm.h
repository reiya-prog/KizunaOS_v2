#pragma once

#include "efi.h"
#include "kernel.h"

extern "C"{
    __attribute__((ms_abi)) void io_hlt(void);
    __attribute__((ms_abi)) void io_cli(void);
    __attribute__((ms_abi)) void io_sti(void);
    __attribute__((ms_abi)) void io_stihlt(void);
    __attribute__((ms_abi)) void int21_handler(void);
    __attribute__((ms_abi)) void default_handler(void);
    __attribute__((ms_abi)) void load_gdt(unsigned long long gdtr[2]);
    __attribute__((ms_abi)) void load_idt(unsigned long long idtr[2]);
    __attribute__((ms_abi)) uint8_t io_read_8(unsigned short addr);
    __attribute__((ms_abi)) uint32_t io_read_32(unsigned short addr);
    __attribute__((ms_abi)) void io_write_8(unsigned short addr, uint8_t value);
    __attribute__((ms_abi)) void io_write_32(unsigned short addr, uint32_t value);
    __attribute__((ms_abi)) void write_ss_selector(unsigned short ss);
    __attribute__((ms_abi)) void write_ds_selector(unsigned short ds);
    __attribute__((ms_abi)) void write_cs_selector(unsigned short cs);
}