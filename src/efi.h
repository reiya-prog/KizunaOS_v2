#pragma once

/*
詳しくはUEFI Manualを参照
*/

#define IN
#define OUT
#define CONST const
#define EFIAPI

#define EFI_FILE_MODE_READ 0x0000000000000001
#define EFI_FILE_MODE_WRITE 0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000

// #define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544f4f42
// #define EFI_FILE_PROTOCOL_REVISION 0x00010000
// #define EFI_FILE_PROTOCOL_REVISION2 0x00020000
// #define EFI_FILE_PROTOCOL_LATEST_REVISION EFI_FILE_PROTOCOL_REVISION2

class EFI
{
public:
    typedef bool BOOLEAN;
    typedef long long INTN;
    typedef unsigned long long UINTN;
    typedef char INT8;
    typedef unsigned char UINT8;
    typedef short INT16;
    typedef unsigned short UINT16;
    typedef int INT32;
    typedef unsigned int UINT32;
    typedef long long INT64;
    typedef unsigned long long UINT64;
    typedef unsigned char CHAR8;
    typedef unsigned short CHAR16;
    typedef void VOID;
    typedef struct
    {
        unsigned int Data1;
        unsigned short Data2;
        unsigned short Data3;
        unsigned char Data4[8];
    } EFI_GUID;
    typedef UINTN EFI_STATUS;
    typedef VOID *EFI_HANDLE;
    typedef VOID *EFI_EVENT;
    typedef UINT64 EFI_LBA;
    typedef UINTN EFI_TPL;
    typedef UINT64 EFI_PHYSICAL_ADDRESS;
    typedef UINT64 EFI_VIRTUAL_ADDRESS;
    // typedef ? EFI_MAC_ADDRESS;
    // typedef ? EFI_IPv4_ADDRESS;
    // typedef ? EFI_IPv6_ADDRESS;
    // typedef ? EFI_IP_ADDRESS;

    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
    struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

    typedef enum
    {
        EFI_SUCCESS,
        EFI_LOAD_ERROR,
        EFI_INVALID_PARAMETER,
        EFI_UNSUPPORTED,
        EFI_BAD_BUFFER_SIZE,
        EFI_BUFFER_TOO_SMALL,
        EFI_NOT_READY,
        EFI_DEVICE_ERROR,
        EFI_WRITE_PROTECTED,
        EFI_OUT_OF_RESOURCES,
        EFI_VOLUME_CORRUPTED,
        EFI_VOLUME_FULL,
        EFI_NO_MEDIA,
        EFI_MEDIA_CHANGED,
        EFI_NOT_FOUND,
        EFI_ACCESS_DENIED,
        EFI_NO_RESPONSE,
        EFI_NO_MAPPING,
        EFI_TIMEOUT,
        EFI_NOT_STARTED,
        EFI_ALREADY_STARTED,
        EFI_ABORTED,
        EFI_ICMP_ERROR,
        EFI_TFTP_ERROR,
        EFI_PROTOCOL_ERROR,
        EFI_INCOMPATIBLE_VERSION,
        EFI_SECURITY_VIOLATION,
        EFI_CRC_ERROR,
        EFI_END_OF_MEDIA,
        NOT_USE_29,
        NOT_USE_30,
        EFI_END_OF_FILE,
        EFI_INVALID_LANGUAGE,
        EFI_COMPROMISED_DATA,
        EFI_IP_ADDRESS_CONFLICT,
        EFI_HTTP_ERROR
    } EFI_STATUS_CODES;

    typedef struct
    {
        UINT16 Year;
        UINT8 Month;
        UINT8 Day;
        UINT8 Hour;
        UINT8 Minute;
        UINT8 Second;
        UINT8 Padl;
        UINT32 Nanosecond;
        INT16 TimeZone;
        UINT8 Daylight;
        UINT8 Pad2;
    } EFI_TIME;

    typedef struct
    {
        UINT16 ScanCode;
        CHAR16 UnicodeChar;
    } EFI_INPUT_KEY;

    typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL
    {
        EFI_STATUS(EFIAPI *Reset)
        (
            IN struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
            IN BOOLEAN ExtendedVerification);
        EFI_STATUS(EFIAPI *ReadKeyStroke)
        (
            IN struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
            OUT EFI_INPUT_KEY *Key);
        EFI_EVENT WaitForKey;
    } EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

    typedef struct
    {
        INT32 MaxMode;
        INT32 Mode;
        INT32 Attribute;
        INT32 CursorColumn;
        INT32 CursorRow;
        BOOLEAN CursorVisible;
    } SIMPLE_TEXT_OUTPUT_MODE;

    typedef struct
    {
        INT32 RelativeMovementX;
        INT32 RelativeMovementY;
        INT32 RelativeMovementZ;
        BOOLEAN LeftBotton;
        BOOLEAN RightBotton;
    } EFI_SIMPLE_POINTER_STATE;

    typedef struct
    {
        UINT64 ResolutionX;
        UINT64 ResolutionY;
        UINT64 ResolutionZ;
        BOOLEAN LeftButton;
        BOOLEAN RightButton;
    } EFI_SIMPLE_POINTER_MODE;

    typedef struct
    {
        UINT32 RedMask;
        UINT32 GreenMask;
        UINT32 BlueMask;
        UINT32 ReservedMask;
    } EFI_PIXEL_BITMASK;

    typedef enum
    {
        PixelRedGreenBlueReserved8BitPerColor,
        PixelBlueGreenRedReserved8BitPerColor,
        PixelBitMask,
        PixelBitOnly,
        PixelFormatMax
    } EFI_GRAPHICS_PIXEL_FORMAT;

    typedef struct
    {
        UINT32 Version;
        UINT32 HorizontalResolution;
        UINT32 VerticalResolution;
        EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
        EFI_PIXEL_BITMASK PixelInformation;
        UINT32 PixelPerScanLine;
    } EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

    typedef struct
    {
        UINT8 Blue;
        UINT8 Green;
        UINT8 Red;
        UINT8 Reserved;
    } EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

    typedef enum
    {
        EfiBltVideoFill,
        EfiBltVideoToBltBuffer,
        EfiBltBufferToVideo,
        EfiVideoToBuffer,
        EfiGraphicsOutputBltOperationMax
    } EFI_GRAPHICS_OUTPUT_BLT_OPERATION;

    typedef struct
    {
        UINT32 MaxMode;
        UINT32 Mode;
        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
        UINTN SizeOfInfo;
        EFI_PHYSICAL_ADDRESS FrameBufferBase;
        UINTN FrameBufferSize;
    } EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

    typedef enum
    {
        AllocateAnyPages,
        AllocateMaxAddress,
        AllocateAddress,
        MaxAllocateType
    } EFI_ALLOCATE_TYPE;

    typedef enum
    {
        EfiReservedMemoryType,
        EfiLoaderCode,
        EfiLoaderData,
        EfiBootServicesCode,
        EfiBootServicesData,
        EfiRuntimeServicesCode,
        EfiRuntimeServicesData,
        EfiConventionalMemory,
        EfiUnusableMemory,
        EfiACPIReclaimMemory,
        EfiACPIMemoryNVS,
        EfiMemoryMappedIO,
        EfiMemoryMappedIOPortSpace,
        EfiPalCode,
        EfiPersistentMemory,
        EfiMaxMemoryType
    } EFI_MEMORY_TYPE;

    typedef struct
    {
        UINT32 Type;
        EFI_PHYSICAL_ADDRESS PhysicalStart;
        EFI_VIRTUAL_ADDRESS VirtualStart;
        UINT64 NumberOfPages;
        UINT64 Attribute;
    } EFI_MEMORY_DESCRIPTOR;

    typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
    {
        EFI_STATUS(EFIAPI *Reset)
        (
            IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
            IN BOOLEAN ExtendedVerification);
        EFI_STATUS(EFIAPI *OutputString)
        (
            IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
            IN CHAR16 *String);
        EFI_STATUS(EFIAPI *TestString)
        (
            IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
            IN CHAR16 *String);
        EFI_STATUS(EFIAPI *QueryMode)
        (
            IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
            IN UINTN ModeNumber,
            OUT UINTN *Columns,
            OUT UINTN *Rows);
        EFI_STATUS(EFIAPI *SetMode)
        (
            IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
            IN UINTN ModeNumber);
        EFI_STATUS(EFIAPI *SetAttribute)
        (
            IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
            IN UINTN Attribute);
        EFI_STATUS(EFIAPI *ClearScreen)
        (
            IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
        EFI_STATUS(EFIAPI *SetCursorPosition)
        (
            IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
            IN UINTN Column,
            IN UINTN Row);
        EFI_STATUS(EFIAPI *EnableCursor)
        (
            IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
            IN BOOLEAN Visible);
        SIMPLE_TEXT_OUTPUT_MODE *Mode;
    } EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

    typedef struct _EFI_SIMPLE_POINTER_PROTOCOL
    {
        EFI_STATUS(EFIAPI *Reset)
        (
            IN _EFI_SIMPLE_POINTER_PROTOCOL *This,
            IN BOOLEAN ExtendedVerification);
        EFI_STATUS(EFIAPI *GetState)
        (
            IN _EFI_SIMPLE_POINTER_PROTOCOL *This,
            IN OUT EFI_SIMPLE_POINTER_STATE *State);
        EFI_EVENT WaitForInput;
        EFI_SIMPLE_POINTER_MODE *Mode;
    } EFI_SIMPLE_POINTER_PROTOCOL;

    typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL
    {
        EFI_STATUS(EFIAPI *QueryMode)
        (
            IN _EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
            IN UINT32 ModeNumber,
            OUT UINTN *SizeOfInfo,
            OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info);
        EFI_STATUS(EFIAPI *SetMode)
        (
            IN _EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
            IN UINT32 ModeNumber);
        EFI_STATUS(EFIAPI *Blt)
        (
            IN _EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
            IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer,
            IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BitOperation,
            IN UINTN SourceX,
            IN UINTN SourceY,
            IN UINTN DestinationX,
            IN UINTN DestinationY,
            IN UINTN Width,
            IN UINTN Height,
            IN UINTN Delta);
        EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
    } EFI_GRAPHICS_OUTPUT_PROTOCOL;

    typedef struct
    {
        UINT64 Size;
        UINT64 FileSize;
        UINT64 PhysicalSize;
        EFI_TIME CreateTime;
        EFI_TIME LastAccessTime;
        EFI_TIME ModificationTime;
        UINT64 Attribute;
        CHAR16 FileName[];
    } EFI_FILE_INFO;

    typedef struct _EFI_FILE_PROTOCOL
    {
        UINT64 Revision;
        EFI_STATUS(EFIAPI *Open)
        (
            IN _EFI_FILE_PROTOCOL *This,
            OUT _EFI_FILE_PROTOCOL **NewHandle,
            IN CHAR16 *FileName,
            IN UINT64 OpenMode,
            IN UINT64 Attributes);
        EFI_STATUS(EFIAPI *Close)
        (
            IN _EFI_FILE_PROTOCOL *This);
        EFI_STATUS(EFIAPI *Delete)
        (
            IN _EFI_FILE_PROTOCOL *This);
        EFI_STATUS(EFIAPI *Read)
        (
            IN _EFI_FILE_PROTOCOL *This,
            IN OUT UINTN *BufferSize,
            OUT VOID *Buffer);
        EFI_STATUS(EFIAPI *Write)
        (
            IN _EFI_FILE_PROTOCOL *This,
            IN OUT INTN *BufferSize,
            IN VOID *Buffer);
        unsigned long long buf[2];
        EFI_STATUS(EFIAPI *GetInfo)
        (
            IN _EFI_FILE_PROTOCOL *This,
            IN EFI_GUID *InformationType,
            IN OUT UINTN *BufferSize,
            OUT VOID *Buffer);
        EFI_STATUS(EFIAPI *SetInfo)
        (
            IN _EFI_FILE_PROTOCOL *This,
            IN EFI_GUID *InformationType,
            IN UINTN BufferSize,
            IN VOID *Buffer);
        EFI_STATUS(EFIAPI *Flush)
        (
            IN _EFI_FILE_PROTOCOL *This);
    } EFI_FILE_PROTOCOL;

    typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
    {
        UINT64 Revision;
        EFI_STATUS(EFIAPI *OpenVolume)
        (
            IN _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
            OUT EFI_FILE_PROTOCOL **Root);
    } EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

    typedef struct
    {
        char buf1[24];
        //
        // Task Priority Services
        //
        unsigned long long buf2[2];

        //
        // Memory Services
        //
        EFI_STATUS(EFIAPI *AllocatePages)
        (
            IN EFI_ALLOCATE_TYPE Type,
            IN EFI_MEMORY_TYPE MemoryType,
            IN UINTN Pages,
            IN OUT EFI_PHYSICAL_ADDRESS *Memory);
        EFI_STATUS(EFIAPI *FreePages)
        (
            IN EFI_PHYSICAL_ADDRESS Memory,
            IN UINTN Pages);
        EFI_STATUS(EFIAPI *GetMemoryMap)
        (
            IN OUT UINTN *MemoryMapSize,
            IN OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
            OUT UINTN *MapKey,
            OUT UINTN *DescriptorSize,
            OUT UINT32 *DescriptorVersion);
        EFI_STATUS(EFIAPI *AllocatePool)
        (
            IN EFI_MEMORY_TYPE PoolType,
            IN UINTN Size,
            OUT VOID **Buffer);
        EFI_STATUS(EFIAPI *FreePool)
        (
            IN VOID *Buffer);
        //
        // Event & Timer Services
        //
        unsigned long long buf4[2];
        EFI_STATUS(EFIAPI *WaitForEvent)
        (
            IN UINTN NumberOfEvents,
            IN EFI_EVENT *Event,
            OUT UINTN *Index);
        unsigned long long buf5[3];

        //
        // Protocol Handler Services
        //
        unsigned long long buf6[9];

        //
        // Image Services
        //
        unsigned long long buf7[4];
        EFI_STATUS(EFIAPI *ExitBootServices)
        (
            IN EFI_HANDLE ImageHandle,
            IN UINTN MapKey);

        //
        // Miscellaneous Services
        //
        unsigned long long buf8[2];
        EFI_STATUS(EFIAPI *SetWatchdogTimer)
        (
            IN UINTN Timeout,
            IN UINT64 WatchdogCode,
            IN UINTN DataSize,
            IN CHAR16 *WatchdogData);

        //
        // DriverSupport Services
        //
        unsigned long long buf9[2];

        //
        // Open and Close Protocol Services
        //
        unsigned long long buf10[3];

        //
        // Library Services
        //
        unsigned long long buf11[2];
        EFI_STATUS(EFIAPI *LocateProtocol)
        (
            IN EFI_GUID *Protocol,
            IN VOID *Registration,
            OUT VOID **Interface);
        unsigned long long buf11_2[2];

        //
        // 32-bit CRC Services
        //
        unsigned long long buf12;

        //
        // Miscellaneous Services
        //
        unsigned long long buf13[3];
    } EFI_BOOT_SERVICES;

    typedef struct
    {
        char buf[36];
        EFI_HANDLE ConsoleInHandle;
        EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
        EFI_HANDLE ConsoleOutHandle;
        EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
        EFI_HANDLE StandardErrorHandle;
        EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
        unsigned long long _buf;
        EFI_BOOT_SERVICES *BootServices;
    } EFI_SYSTEM_TABLE;

    EFI(){};
    EFI(EFI_SYSTEM_TABLE *ST)
    {
        this->system_table_ = ST;
    }

    void initEFI();
    void setSystemTable(EFI_SYSTEM_TABLE *ST)
    {
        this->system_table_ = ST;
    }
    EFI_SYSTEM_TABLE *getSystemTable()
    {
        return this->system_table_;
    }
    EFI_SIMPLE_POINTER_PROTOCOL *getSimplePointerProtocol()
    {
        return this->simple_pointer_protocol_;
    }
    EFI_GRAPHICS_OUTPUT_PROTOCOL *getGraphicsOutputProtocol()
    {
        return this->graphics_output_protocol_;
    }
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *getSimpleFileSystemProtocol()
    {
        return this->simple_file_system_protocol_;
    }

private:
    EFI_SYSTEM_TABLE *system_table_ = nullptr;
    EFI_SIMPLE_POINTER_PROTOCOL *simple_pointer_protocol_ = nullptr;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graphics_output_protocol_ = nullptr;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *simple_file_system_protocol_ = nullptr;
};