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
	-I $(HOMEDIR)/x86_64-elf/include \
	--target=x86_64-pc-win32-coff \
	-fno-stack-protector -fno-exceptions -fshort-wchar \
	-nostdlibinc -mno-red-zone \
	-Wall -Wextra -Wpedantic

LOADER_CPPFLAGS = \
	-I $(HOMEDIR)/x86_64-elf/include \
	-D_LIBCPP_HAS_NO_THREADS \
	--target=x86_64-pc-win32-coff \
	-fno-stack-protector -fno-exceptions -fshort-wchar \
	-nostdlibinc -nostdlib -mno-red-zone \
	-Wall -Wextra -Wpedantic -Qunused-arguments -Wno-keyword-macro -Wno-char-subscripts -Wno-int-to-pointer-cast \
	-Wno-c99-extensions -Wno-unused-parameter -Wno-unused-variable -Wno-writable-strings -Wno-macro-redefined \
	-fno-builtin \
	-std=c++17

KERNEL_CPPFLAGS = \
	-I $(HOMEDIR)/x86_64-elf/include -I $(HOMEDIR)/x86_64-elf/include/c++/v1 \
	-D__ELF__ -D_LIBCPP_HAS_NO_THREADS \
	--target=x86_64-unknown-none-elf \
	-fno-stack-protector -fno-exceptions -fshort-wchar \
	-nostdlibinc -ffreestanding -mno-red-zone \
	-Wall -Wextra -Wpedantic -Qunused-arguments -Wno-keyword-macro -Wno-char-subscripts -Wno-int-to-pointer-cast \
	-Wno-c99-extensions -Wno-unused-parameter -Wno-unused-variable -Wno-writable-strings -Wno-macro-redefined -Wno-sign-compare\
	-fno-builtin \
	-g \
	-std=c++17

LD_LINK = /usr/bin/lld-link-10
LD_LLD = /usr/bin/ld.lld
LOADER_LDFLAGS = \
	-subsystem:efi_application -nodefaultlib \
	-entry:efi_main

KERNEL_LDFLAGS = \
	-T kernel.ld -L$(HOMEDIR)/x86_64-elf/lib -lc++ -lc++abi -lm -lc -static

QEMU = qemu-system-x86_64
OVMF = ovmf/bios64.bin
QEMUflags = \
	-bios $(OVMF) -drive format=raw,file=fat:rw:$(OUTDIR)

LOADER_SRCS = \
	boot_loader.cpp efi_main.cpp efi.cpp efi_kernel_loader.cpp loader_asm.s std_func.cpp graphics.cpp
KERNEL_SRCS = \
	kernel.cpp kernel_asm.s std_func.cpp graphics.cpp libc_support.c

SRCS = $(wildcard $(SRCDIR)/*.cpp)
LOADER_OBJS := $(addprefix $(OBJDIR)/,$(addsuffix .o, $(basename $(notdir $(LOADER_SRCS)))))
KERNEL_OBJS := $(addprefix $(OBJDIR)/,$(addsuffix .elf.o, $(basename $(KERNEL_SRCS))))
DEPS := $(addprefix $(OBJDIR)/,$(patsubst $(SRCDIR)/%.cpp,%.d,$(SRCS)))

.PHONY: all clean
all: Makefile $(APPDIR) $(OBJDIR) $(TOOLS) $(LOADER_TARGET) $(KERNEL_TARGET)

$(APPDIR):
	mkdir -p $(APPDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

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

$(KERNEL_TARGET): $(KERNEL_OBJS) kernel.ld
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

loader: Makefile $(OBJDIR) $(LOADER_TARGET)

run: $(TARGET)
	$(QEMU) $(QEMUflags)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(OUTDIR)

-include $(DEPS)