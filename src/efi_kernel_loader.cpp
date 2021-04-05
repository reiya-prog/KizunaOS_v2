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
    unsigned long long file_buf[100];
    unsigned stat = file->GetInfo(file, &EFI_FILE_INFO_GUID, &file_size, file_buf);
    file_info = (EFI::EFI_FILE_INFO *)file_buf;
    return file_info->FileSize;
}

void LoadKernel(EFI::EFI_HANDLE image_handle, FrameBuffer *frame_buffer)
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
    unsigned long long kernel_size = 0;
    kernel_size = EFIFileSizeof(kernel_file);
    EFI::EFI_PHYSICAL_ADDRESS kernel_addr = 0x00100000lu;
    EFI::EFI_PHYSICAL_ADDRESS kernel_tmp_addr = 0x01100000lu;
    is_success = gEFI->getSystemTable()->BootServices->AllocatePages(
        EFI::AllocateAddress,
        EFI::EfiLoaderData,
        (kernel_size + 0xfff) / 0x1000,
        &kernel_addr);
    if (is_success == EFI::EFI_SUCCESS)
    {
        is_success = gEFI->getSystemTable()->BootServices->AllocatePages(
            EFI::AllocateAddress,
            EFI::EfiLoaderData,
            (kernel_size + 0xfff) / 0x1000,
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
    unsigned long long elf_header_size = sizeof(elf_header);
    EFIPrint(L"Check ELF magic number in ELF header...");
    if ((EFI::CHAR8 *)elf_header->e_ident[0] == (EFI::CHAR8 *)'\x7f' &&
        (EFI::CHAR8 *)elf_header->e_ident[1] == (EFI::CHAR8 *)'E' &&
        (EFI::CHAR8 *)elf_header->e_ident[2] == (EFI::CHAR8 *)'L' &&
        (EFI::CHAR8 *)elf_header->e_ident[3] == (EFI::CHAR8 *)'F')
    {
        EFIPrint(L"done.\r\n");
        EFIPrint(L"This file is ELF file.\r\n");
    }
    else
    {
        EFIPrint(L"\r\n    Header's magic number does not match ELF\r\n");
        EFIPrint(L"    Cannot load kernel file.\r\n");
        return;
    }
    EFIPrint(L"ELF header successfully checked!\r\n");

    EFIPrint(L"Relocate ELF program data...");
    Elf64_Phdr *elf_program_headers = reinterpret_cast<Elf64_Phdr *>(kernel_tmp_addr + elf_header->e_phoff);
    Elf64_Shdr *elf_section_headers = reinterpret_cast<Elf64_Shdr *>(kernel_tmp_addr + elf_header->e_shoff);

    for (unsigned int i = 0; i < elf_header->e_phnum; ++i)
    {
        Elf64_Phdr program_header = elf_program_headers[i];
        if (program_header.p_type != PT_LOAD)
            continue;

        // memcpy(dst, src, size)
        memcpy(reinterpret_cast<void *>(kernel_addr + program_header.p_vaddr),
               reinterpret_cast<void *>(kernel_tmp_addr + program_header.p_offset),
               program_header.p_filesz);
        // memset(dst, value, size);
        memset(reinterpret_cast<void *>(kernel_addr + program_header.p_vaddr + program_header.p_filesz),
               0,
               program_header.p_memsz - program_header.p_filesz);
    }
    EFIPrint(L"done.\r\n");
    EFIPrint(L"ELF sections successfully relocated!\r\n");

    EFIPrint(L"Search symbol table section...");
    Elf64_Shdr symbol_table_sections;
    for (unsigned int i = 0; i < elf_header->e_shnum; ++i)
    {
        Elf64_Shdr search_section = elf_section_headers[i];
        if (search_section.sh_type != SHT_SYMTAB)
            continue;
        symbol_table_sections = search_section;
        break;
    }
    EFIPrint(L"done.\r\n");

    EFIPrint(L"Relocate dynamic symbols...");
    for (unsigned int sh_i = 0; sh_i < elf_header->e_shnum; ++sh_i)
    {
        Elf64_Shdr section_header = elf_section_headers[sh_i];
        if (section_header.sh_type != SHT_RELA)
            continue;
        for (unsigned int rl_i = 0; rl_i < section_header.sh_size / section_header.sh_entsize; ++rl_i)
        {
            Elf64_Rela *rela = (Elf64_Rela *)(kernel_tmp_addr + section_header.sh_offset + section_header.sh_entsize * rl_i);
            Elf64_Sym *target_symbol = nullptr;
            for (unsigned int st_i = 0; st_i < symbol_table_sections.sh_size / symbol_table_sections.sh_entsize; ++st_i)
            {
                Elf64_Sym *search_symbol = (Elf64_Sym *)(kernel_tmp_addr + symbol_table_sections.sh_offset + symbol_table_sections.sh_entsize * st_i);
                if (search_symbol->st_value != static_cast<unsigned long long>(rela->r_addend))
                    continue;
                target_symbol = search_symbol;
                break;
            }
            unsigned long long *memory_to = (unsigned long long *)(kernel_addr + rela->r_offset);
            *memory_to = kernel_addr + target_symbol->st_value;
        }
    }
    EFIPrint(L"done.\r\n");

    unsigned long long entry_point = elf_header->e_entry + kernel_addr;
    BootStruct boot_info;
    boot_info.frame_buffer = *frame_buffer;
    kernel_file->Close(kernel_file);
    root_dir->Close(root_dir);
    gEFI->getSystemTable()->BootServices->FreePages(kernel_tmp_addr, (kernel_size + 0xfff) / 0x1000);
    gEFI->getSystemTable()->ConOut->ClearScreen(gEFI->getSystemTable()->ConOut);

    EFI::EFI_MEMORY_DESCRIPTOR *memory_map = nullptr;
    EFI::UINTN memory_map_size = 0;
    EFI::UINTN map_key, descriptor_size;
    EFI::UINT32 descriptor_version;
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
        boot_info.frame_buffer.frame_buffer_base = reinterpret_cast<unsigned long long *>(gEFI->getGraphicsOutputProtocol()->Mode->FrameBufferBase);
        is_success = gEFI->getSystemTable()->BootServices->ExitBootServices(image_handle, map_key);
    } while (is_success != EFI::EFI_SUCCESS);

    unsigned stack_pointer = 0x7f0000lu;
    jump_entry(&boot_info, entry_point, stack_pointer);
}