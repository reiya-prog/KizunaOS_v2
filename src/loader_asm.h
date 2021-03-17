#pragma once

extern "C"
{
    __attribute__((ms_abi)) void sleep_hlt(void);
    __attribute__((ms_abi)) void jump_entry(BootStruct *boot_struct, unsigned long long entry_addr, unsigned long long stack_pointer);
}