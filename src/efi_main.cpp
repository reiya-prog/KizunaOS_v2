#include "boot_loader.h"

/*
C++はマングリングにより正常にmain関数として呼び出すことができないので、
Cで書かれたmain関数を踏み台としてC++の関数を呼び出す。
*/
extern "C"
{
    void efi_main(
        IN EFI::EFI_HANDLE ImageHandle,
        IN EFI::EFI_SYSTEM_TABLE *SystemTable)
    {
        BootLoader(ImageHandle, SystemTable);
    }
}