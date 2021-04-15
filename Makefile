LOADER_NAME := BOOTX64.EFI
KERNEL_NAME := KizunaOS.ELF
SRCDIR := src
OBJDIR := build
OUTDIR := app
APPDIR := $(OUTDIR)/EFI/BOOT
HOMEDIR := $(shell echo $$HOME)
WORKDIR := $(shell pwd)

LOADER_TARGET = $(APPDIR)/$(LOADER_NAME)
KERNEL_TARGET = $(OUTDIR)/$(KERNEL_NAME)

CC = clang++
LOADER_CFLAGS = \
	--target=x86_64-pc-win32-coff \
	-fno-stack-protector -fno-exceptions -fshort-wchar \
	-mno-red-zone \
	-Wall -Wextra -Wpedantic -Wno-writable-strings

LOADER_CPPFLAGS = \
	--target=x86_64-pc-win32-coff \
	-fno-stack-protector -fno-exceptions -fshort-wchar \
	-mno-red-zone \
	-Wall -Wno-writable-strings \
	-fno-builtin \
	-std=c++17

KERNEL_CPPFLAGS = \
	-I $(HOMEDIR)/x86_64-elf/include -I $(HOMEDIR)/x86_64-elf/include/c++/v1 -nostdlibinc \
	-D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS \
	--target=x86_64-unknown-none-elf -mno-red-zone -Wall \
	-fno-stack-protector -fno-exceptions -fshort-wchar -fno-rtti -fno-builtin -ffreestanding \
	-g \
	-std=c++17

LD_LINK = /usr/bin/lld-link-10
LD_LLD = /usr/bin/ld.lld
LOADER_LDFLAGS = \
	-subsystem:efi_application -nodefaultlib \
	-entry:efi_main

KERNEL_LDFLAGS = \
	-L $(HOMEDIR)/x86_64-elf/lib --entry kernel_start --static -z norelro --image-base 0x100000 -lc++ -lc++abi -lm -lc

QEMU = qemu-system-x86_64
OVMF = ovmf/bios64.bin
QEMUflags = \
	-bios $(OVMF) -drive format=raw,file=fat:rw:$(OUTDIR)

LOADER_SRCS = \
	Loader/boot_loader.cpp Loader/efi_main.cpp Loader/efi.cpp Loader/efi_kernel_loader.cpp Loader/loader_asm.s
KERNEL_SRCS = \
	Kernel/kernel.cpp Kernel/kernel_asm.s Kernel/std_func.cpp Kernel/graphics.cpp Kernel/assets.cpp Kernel/libc_support.cpp Kernel/console.cpp

SRCS = $(wildcard $(SRCDIR)/*.cpp)
LOADER_OBJS := $(addprefix $(OBJDIR)/,$(addsuffix .o, $(basename $(LOADER_SRCS))))
KERNEL_OBJS := $(addprefix $(OBJDIR)/,$(addsuffix .elf.o, $(basename $(KERNEL_SRCS))))
DEPS := $(addprefix $(OBJDIR)/,$(patsubst $(SRCDIR)/%.cpp,%.d,$(SRCS)))

.PHONY: all clean
all: Makefile $(APPDIR) $(OBJDIR) $(TOOLS) $(LOADER_TARGET) $(KERNEL_TARGET)

$(APPDIR):
	mkdir -p $(APPDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)/Loader
	mkdir -p $(OBJDIR)/Kernel

$(OBJDIR)/%.o:$(SRCDIR)/%.s
	$(CC) $(LOADER_CFLAGS) -o $@ -c $<

$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	$(CC) $(LOADER_CPPFLAGS) -o $@ -c $<

$(OBJDIR)/%.elf.o:$(SRCDIR)/%.s
	$(CC) $(KERNEL_CFLAGS) -o $@ -c $<

$(OBJDIR)/%.elf.o:$(SRCDIR)/%.c
	$(CC) $(KERNEL_CFLAGS) -o $@ -c $<

$(OBJDIR)/%.elf.o:$(SRCDIR)/%.cpp
	$(CC) $(KERNEL_CPPFLAGS) -o $@ -c $<

$(LOADER_TARGET): $(LOADER_OBJS)
	$(LD_LINK) $(LOADER_LDFLAGS) -out:$(LOADER_TARGET) $(LOADER_OBJS)

$(KERNEL_TARGET): $(KERNEL_OBJS)
	$(LD_LLD) $(KERNEL_LDFLAGS) -o $(KERNEL_TARGET) $(KERNEL_OBJS)

$(TOOLS):
	mkdir -p work
	cd work
	git clone --depth 1 --branch release/10.x https://github.com/llvm/llvm-project.git
	mkdir -p build_libcxxabi
	cd build_libcxxabi
	cmake -G "Unix Makefiles" \
		-DCMAKE_INSTALL_PREFIX=$(WORKDIR)/x86_64-elf \
		-DCMAKE_CXX_COMPILER=clang++ \
		-DCMAKE_CXX_FLAGS="-I$(WORKDIR)/x86_64-elf/include -nostdlibinc -O2 -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS -D_LIBCPP_HAS_NO_THREADS" \
		-DCMAKE_C_COMPILER=clang \
		-DCMAKE_C_FLAGS="-I$(WORKDIR)/x86_64-elf/include -nostdlibinc -O2 -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS -D_LIBCPP_HAS_NO_THREADS" \
		-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
		-DCMAKE_BUILD_TYPE=Release \
		-DLIBCXXABI_LIBCXX_INCLUDES="$(WORKDIR)/work/llvm-project/libcxx/include" \
		-DLIBCXXABI_ENABLE_EXCEPTIONS=False \
		-DLIBCXXABI_ENABLE_THREADS=False \
		-DLIBCXXABI_TARGET_TRIPLE=x86_64-elf \
		-DLIBCXXABI_ENABLE_SHARED=False \
		-DLIBCXXABI_ENABLE_STATIC=True \
		$(WORKDIR)/work/llvm-project/libcxxabi
	mkdir -p build_libcxx
	cd build_libcxx
	cmake -G "Unix Makefiles" \
		-DCMAKE_INSTALL_PREFIX=$(WORKDIR)/x86_64-elf \
		-DCMAKE_CXX_COMPILER=clang++ \
		-DCMAKE_CXX_FLAGS="-I$(WORKDIR)/x86_64-elf/include -nostdlibinc -O2 -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS" \
		-DCMAKE_CXX_COMPILER_TARGET=x86_64-elf \
		-DCMAKE_C_COMPILER=clang \
		-DCMAKE_C_FLAGS="-I$(WORKDIR)/x86_64-elf/include -nostdlibinc -O2 -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS" \
		-DCMAKE_C_COMPILER_TARGET=x86_64-elf \
		-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
		-DCMAKE_BUILD_TYPE=Release \
		-DLIBCXX_CXX_ABI=libcxxabi \
		-DLIBCXX_CXX_ABI_INCLUDE_PATHS="$(WORKDIR)/work/llvm-project/libcxxabi/include" \
		-DLIBCXX_CXX_ABI_LIBRARY_PATH="$(WORKDIR)/x86_64-elf/lib" \
		-DLIBCXX_ENABLE_EXCEPTIONS=False \
		-DLIBCXX_ENABLE_FILESYSTEM=False \
		-DLIBCXX_ENABLE_MONOTONIC_CLOCK=False \
		-DLIBCXX_ENABLE_RTTI=False \
		-DLIBCXX_ENABLE_THREADS=False \
		-DLIBCXX_ENABLE_SHARED=False \
		-DLIBCXX_ENABLE_STATIC=True \
		$(WORKDIR)/work/llvm-project/libcxx

tools: Makefile $(TOOLS)

loader: Makefile $(OBJDIR) $(APPDIR) $(LOADER_TARGET)

kernel: Makefile $(OBJDIR) $(APPDIR) $(KERNEL_TARGET)

run: $(TARGET)
	$(QEMU) $(QEMUflags)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(OUTDIR)

-include $(DEPS)