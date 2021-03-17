#include "main.h"

#define KERNEL_NAME L"KizunaOS.ELF"

void EFIPrint(EFI *efi, EFI::CHAR16 *str)
{
    efi->getSystemTable()->ConOut->OutputString(efi->getSystemTable()->ConOut, str);
}

unsigned long long EFIFileSizeof(EFI *efi, EFI::EFI_FILE_PROTOCOL *file)
{
    EFI::EFI_GUID EFI_FILE_INFO_GUID = {0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
    EFI::EFI_FILE_INFO *file_info;
    unsigned long long file_size = 0;
    unsigned long long file_buf[100];
    unsigned stat = file->GetInfo(file, &EFI_FILE_INFO_GUID, &file_size, file_buf);
    file_info = (EFI::EFI_FILE_INFO *)file_buf;
    return file_info->FileSize;
}

void LoadKernel(EFI::EFI_HANDLE image_handle, EFI *efi, FrameBuffer *frame_buffer)
{

    EFI::EFI_STATUS is_success = 0;
    EFI::EFI_FILE_PROTOCOL *root_dir, *kernel_file;


    EFIPrint(efi, L"Open root directory and kernel file.\r\n");
    EFIPrint(efi, L"\tOpen root directory...");
    is_success = efi->getSimpleFileSystemProtocol()->OpenVolume(efi->getSimpleFileSystemProtocol(), &root_dir);
    if (is_success == EFI::EFI_SUCCESS)
    {
        EFIPrint(efi, L"done.\r\n");
    }
    else
    {
        EFIPrint(efi, L"\r\n\tCannot open root directory.\r\n");
        EFIPrint(efi, L"\tAn unexpected error has occurred.\r\n");
        return;
    }

    EFIPrint(efi, L"\tLoad kernel file...");
    is_success = root_dir->Open(root_dir, &kernel_file, (EFI::CHAR16 *)KERNEL_NAME, EFI_FILE_MODE_READ, 0);
    if (is_success == EFI::EFI_SUCCESS)
    {
        EFIPrint(efi, L"done.\r\n");
    }
    else
    {
        EFIPrint(efi, L"\r\n\tCannot open kernel file.\r\n");
        EFIPrint(efi, L"\tAn unexpected error has occurred.\r\n");
        return;
    }
    EFIPrint(efi, L"Kernel file successfully opened.\r\n");


    EFIPrint(efi, L"Allocate memory and read kernel data.\r\n"):
    EFIPrint(efi, L"\tAllocate memory to 0x100000...");
    unsigned long long kernel_size = 0;
    kenrel_size = EFIFileSizeof(efi, kernel_file);
    EFI::EFI_PHYSICAL_ADDRESS kernel_addr = 0x00100000lu;
    EFI::EFI_PHYSICAL_ADDRESS kernel_tmp_addr = 0x01100000lu;
    is_success = efi->getSystemTable()->BootServices->AllocatePages(EFI::AllocateAddress, EFI::EfiLoaderData, (kernel_size + 0xfff) / 0x1000, &kernel_addr);
    if (is_success == EFI::EFI_SUCCESS)
    {
        EFIPrint(efi, L"done.\r\n");
    }
    else
    {
        EFIPrint(efi, L"\r\n\tCannot allocate memory to 0x100000.\r\n");
        EFIPrint(efi, L"\tAn unexpected error has occurred.\r\n");
        return;
    }

    EFIPrint(efi, "\tRead kernel data to memory...");
    is_success = kernel_file->Read(kernel_file, &kernel_size, reinterpret_cast<EFI::VOID *>(kernel_tmp_addr));
    if(is_success == EFI::EFI_SUCCESS){
        EFIPrint(efi, L"done.\r\n");
    }else{
        EFIPrint(efi, L"\r\n\tCannot read kernel data to memory.\r\n");
        EFIPrint(efi, L"\tAn unexpected error has occurred.\r\n");
        return;
    }
    EFIPrint(efi, L"Kernel data successfully stored to memory.\r\n");


    
}