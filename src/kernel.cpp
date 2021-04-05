#include "main.h"

extern "C"
{
    void kernel_start(BootStruct *boot_info)
    {
        io_hlt();
    }
}