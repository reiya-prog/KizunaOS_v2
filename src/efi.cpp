#include "main.h"

EFI::EFI_GUID SimplePointerProtocol_GUID = {
    0x31878c87, 0xb75, 0x11d5, {0x9a, 0x4f, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}};

EFI::EFI_GUID SimpleFileSystemProtocol_GUID = {
    0x0964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

EFI::EFI_GUID GraphicsOutputProtocol_GUID = {
    0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};

void EFI::initEFI()
{
    SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, nullptr);
    SystemTable->ConOut->SetMode(SystemTable->ConOut, SystemTable->ConOut->Mode->MaxMode - 1);
    SystemTable->BootServices->LocateProtocol(&GraphicsOutputProtocol_GUID, nullptr, reinterpret_cast<void **>(&this->GraphicsOutputProtocol));
    SystemTable->BootServices->LocateProtocol(&SimplePointerProtocol_GUID, nullptr, reinterpret_cast<void **>(&this->SimplePointerProtocol));
    SystemTable->BootServices->LocateProtocol(&SimpleFileSystemProtocol_GUID, nullptr, reinterpret_cast<void **>(&this->SimpleFileSystemProtocol));
}
