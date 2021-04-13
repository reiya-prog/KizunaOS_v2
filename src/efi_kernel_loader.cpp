#include "main.h"

#define KERNEL_NAME L"KizunaOS.ELF"

EFI::EFI_GUID EFI_FILE_INFO_GUID = {0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

void EFIPrint(wchar_t *str)
{
    gEFI->getSystemTable()->ConOut->OutputString(gEFI->getSystemTable()->ConOut, (EFI::CHAR16 *)str);
}

unsigned long long EFIFileSizeof(EFI::EFI_FILE_PROTOCOL *file)
{
    EFI::EFI_FILE_INFO *file_info;
    unsigned long long file_size = sizeof(EFI::EFI_FILE_INFO) + sizeof(KERNEL_NAME);
    unsigned long long file_buf[200];
    unsigned stat = file->GetInfo(file, &EFI_FILE_INFO_GUID, &file_size, file_buf);
    file_info = (EFI::EFI_FILE_INFO *)file_buf;
    return file_info->FileSize;
}

extern "C" void memcpy(void *dst, const void *src, int size)
{
    char *p1 = reinterpret_cast<char *>(dst);
    const char *p2 = reinterpret_cast<const char *>(src);
    while (size-- > 0)
        *p1++ = *p2++;
}

void CalcLoadAddressRange(Elf64_Ehdr *ehdr, EFI::UINT64 *first, EFI::UINT64 *last)
{
    Elf64_Phdr *phdr = (Elf64_Phdr *)((EFI::UINT64)ehdr + ehdr->e_phoff);
    *first = 1000000000000000;
    *last = 0;
    for (Elf64_Half i = 0; i < ehdr->e_phnum; ++i)
    {
        if (phdr[i].p_type != PT_LOAD)
            continue;
        *first = min(*first, phdr[i].p_vaddr);
        *last = max(*last, phdr[i].p_vaddr + phdr[i].p_memsz);
    }
}

void LoadKernel(EFI::EFI_HANDLE image_handle)
{

    EFI::EFI_STATUS is_success = 0;
    EFI::EFI_FILE_PROTOCOL *root_dir, *kernel_file;

    EFIPrint(L"Open root directory and kernel file.\r\n");
    EFIPrint(L"    Open root directory...");
    is_success = gEFI->getSimpleFileSystemProtocol()->OpenVolume(gEFI->getSimpleFileSystemProtocol(), &root_dir);
    if (is_success == EFI::EFI_SUCCESS)
    {
        EFIPrint(L"done.\r\n");
    }
    else
    {
        EFIPrint(L"\r\n    Cannot open root directory.\r\n");
        EFIPrint(L"    An unexpected error has occurred.\r\n");
        return;
    }

    EFIPrint(L"    Load kernel file...");
    is_success = root_dir->Open(root_dir, &kernel_file, (EFI::CHAR16 *)KERNEL_NAME, EFI_FILE_MODE_READ, 0);
    if (is_success == EFI::EFI_SUCCESS)
    {
        EFIPrint(L"done.\r\n");
    }
    else
    {
        EFIPrint(L"\r\n    Cannot open kernel file.\r\n");
        EFIPrint(L"    An unexpected error has occurred.\r\n");
        return;
    }
    EFIPrint(L"Kernel file successfully opened.\r\n");

    EFIPrint(L"Allocate memory and read kernel data.\r\n");
    EFIPrint(L"    Allocate memory to 0x100000...");

    unsigned long long kernel_first_addr, kernel_last_addr;
    unsigned long long kernel_size = 0;
    kernel_size = EFIFileSizeof(kernel_file);
    void *kernel_tmp_addr;
    is_success = gEFI->getSystemTable()->BootServices->AllocatePool(
        EFI::EfiLoaderData,
        kernel_size,
        &kernel_tmp_addr);
    if (is_success == EFI::EFI_SUCCESS)
    {
        EFIPrint(L"done.\r\n");
    }
    else
    {
        EFIPrint(L"\r\n    Cannot allocate memory to 0x100000.\r\n");
        EFIPrint(L"    An unexpected error has occurred.\r\n");
        return;
    }
    EFIPrint(L"    Read kernel data to memory...");

    is_success = kernel_file->Read(kernel_file, &kernel_size, reinterpret_cast<EFI::VOID *>(kernel_tmp_addr));
    if (is_success == EFI::EFI_SUCCESS)
    {
        EFIPrint(L"done.\r\n");
    }
    else
    {
        EFIPrint(L"\r\n    Cannot read kernel data to memory.\r\n");
        EFIPrint(L"    An unexpected error has occurred.\r\n");
        return;
    }
    EFIPrint(L"Kernel data successfully stored to memory.\r\n");

    Elf64_Ehdr *elf_header = reinterpret_cast<Elf64_Ehdr *>(kernel_tmp_addr);
    CalcLoadAddressRange(elf_header, &kernel_first_addr, &kernel_last_addr);
    is_success = gEFI->getSystemTable()->BootServices->AllocatePages(
        EFI::AllocateAddress,
        EFI::EfiLoaderData,
        (kernel_last_addr - kernel_first_addr + 0xfff) / 0x1000,
        &kernel_first_addr);

    EFIPrint(L"Check ELF magic number in ELF header...");
    if (reinterpret_cast<EFI::CHAR8 *>(elf_header->e_ident[0]) == (EFI::CHAR8 *)'\x7f' &&
        reinterpret_cast<EFI::CHAR8 *>(elf_header->e_ident[1]) == (EFI::CHAR8 *)'E' &&
        reinterpret_cast<EFI::CHAR8 *>(elf_header->e_ident[2]) == (EFI::CHAR8 *)'L' &&
        reinterpret_cast<EFI::CHAR8 *>(elf_header->e_ident[3]) == (EFI::CHAR8 *)'F')
    {
        EFIPrint(L"done.\r\n");
        EFIPrint(L"    This file is ELF file.\r\n");
    }
    else
    {
        EFIPrint(L"\r\n    Header's magic number does not match ELF\r\n");
        EFIPrint(L"    Cannot load kernel file.\r\n");
        return;
    }
    EFIPrint(L"ELF header successfully checked!\r\n");

    EFIPrint(L"Relocate ELF program data...");
    Elf64_Phdr *elf_program_headers = reinterpret_cast<Elf64_Phdr *>((uint64_t)elf_header + elf_header->e_phoff);
    Elf64_Shdr *elf_section_headers = reinterpret_cast<Elf64_Shdr *>((uint64_t)elf_header + elf_header->e_shoff);

    for (Elf64_Half i = 0; i < elf_header->e_phnum; ++i)
    {
        if (elf_program_headers[i].p_type != 1)
            continue;
        // memcpy(dst, src, size)
        memcpy(reinterpret_cast<void *>(elf_program_headers[i].p_vaddr),
               reinterpret_cast<void *>((uint64_t)elf_header + elf_program_headers[i].p_offset),
               elf_program_headers[i].p_filesz);
        // memset(dst, value, size);
        memset(reinterpret_cast<void *>(elf_program_headers[i].p_vaddr + elf_program_headers[i].p_filesz),
               0,
               elf_program_headers[i].p_memsz - elf_program_headers[i].p_filesz);
    }
    EFIPrint(L"done.\r\n");
    EFIPrint(L"ELF sections successfully relocated!\r\n");

    // EFIPrint(L"Search symbol table section...");
    // Elf64_Shdr symbol_table_sections;
    // for (unsigned int i = 0; i < elf_header->e_shnum; ++i)
    // {
    //     Elf64_Shdr search_section = elf_section_headers[i];
    //     if (search_section.sh_type != SHT_SYMTAB)
    //         continue;
    //     symbol_table_sections = search_section;
    //     break;
    // }
    // EFIPrint(L"done.\r\n");

    // EFIPrint(L"Relocate dynamic symbols...");
    // for (unsigned int sh_i = 0; sh_i < elf_header->e_shnum; ++sh_i)
    // {
    //     Elf64_Shdr section_header = elf_section_headers[sh_i];
    //     if (section_header.sh_type != SHT_RELA)
    //         continue;
    //     for (unsigned int rl_i = 0; rl_i < section_header.sh_size / section_header.sh_entsize; ++rl_i)
    //     {
    //         Elf64_Rela *rela = (Elf64_Rela *)(elf_header + section_header.sh_offset + section_header.sh_entsize * rl_i);
    //         Elf64_Sym *target_symbol = nullptr;
    //         for (unsigned int st_i = 0; st_i < symbol_table_sections.sh_size / symbol_table_sections.sh_entsize; ++st_i)
    //         {
    //             Elf64_Sym *search_symbol = (Elf64_Sym *)(elf_header + symbol_table_sections.sh_offset + symbol_table_sections.sh_entsize * st_i);
    //             if (search_symbol->st_value != static_cast<unsigned long long>(rela->r_addend))
    //                 continue;
    //             target_symbol = search_symbol;
    //             break;
    //         }
    //         unsigned long long *memory_to = (unsigned long long *)(kernel_first_addr + rela->r_offset);
    //         *memory_to = kernel_first_addr + target_symbol->st_value;
    //     }
    // }
    // EFIPrint(L"done.\r\n");

    uint64_t entry_addr = *(uint64_t *)(kernel_first_addr + 24);
    kernel_file->Close(kernel_file);
    root_dir->Close(root_dir);
    gEFI->getSystemTable()->BootServices->FreePool(kernel_tmp_addr);
    EFIPrint(L"Start to exit boot services.\r\n");

    EFI::EFI_MEMORY_DESCRIPTOR *memory_map = nullptr;
    EFI::UINTN memory_map_size = 0;
    EFI::UINTN map_key, descriptor_size;
    EFI::UINT32 descriptor_version;
    gEFI->getSystemTable()->ConOut->ClearScreen(gEFI->getSystemTable()->ConOut);
    do
    {
        is_success = gEFI->getSystemTable()->BootServices->GetMemoryMap(&memory_map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);
        while (is_success == EFI::EFI_BUFFER_TOO_SMALL)
        {
            if (memory_map)
                gEFI->getSystemTable()->BootServices->FreePool(memory_map);
            is_success = gEFI->getSystemTable()->BootServices->AllocatePool(EFI::EfiLoaderData, memory_map_size, reinterpret_cast<EFI::VOID **>(&memory_map));
            is_success = gEFI->getSystemTable()->BootServices->GetMemoryMap(&memory_map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);
        }
        is_success = gEFI->getSystemTable()->BootServices->ExitBootServices(image_handle, map_key);
    } while (is_success != EFI::EFI_SUCCESS);

    uint8_t* fb = (uint8_t *) gEFI->getGraphicsOutputProtocol()->Mode->FrameBufferBase;
    FrameBuffer frame_buffer = {
        fb,
        gEFI->getGraphicsOutputProtocol()->Mode->Info->PixelsPerScanLine,
        gEFI->getGraphicsOutputProtocol()->Mode->Info->HorizontalResolution,
        gEFI->getGraphicsOutputProtocol()->Mode->Info->VerticalResolution,
        (PixelFormat)0};

    switch (gEFI->getGraphicsOutputProtocol()->Mode->Info->PixelFormat)
    {
    case EFI::PixelRedGreenBlueReserved8BitPerColor:
        frame_buffer.pixel_format = kPixelRGB8BitPerColor;
        break;
    case EFI::PixelBlueGreenRedReserved8BitPerColor:
        frame_buffer.pixel_format = kPixelBGR8BitPerColor;
        break;
    default:
        EFIPrint(L"An unexpected error has occurred.\r\n");
        return;
    }

    unsigned stack_pointer = 0x7f0000lu;
    //         $rcx           $rdx        $r8
    jump_entry(&frame_buffer, entry_addr, stack_pointer);
}