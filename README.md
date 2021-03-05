# Kizuna OS
ぼちぼちがんばりたい
## Requirements
Windows wsl2 Ubuntu 20.04.1 LTSを使用しています。
でもwsl2のQEMUはCUI実行のみなので実行はWindows上でやっていたりしてます。
### common
#### clang++
```
sudo apt install clang -y
```
#### OVMF
```
sudo apt install ovmf
```
#### g++
クロスコンパイル用。C++のためg++を使用、Cならgcc。
```
sudo apt install g++-mingw-w64-x86-64
```
#### lld
```
sudo apt install lld-6.0
```
### standard library
#### newlib
```
mkdir -p $HOME/work
cd $HOME/work
git clone git://sourceware.org/git/newlib-cygwin.git
cd newlib-cygwin
git checkout newlib-2_5_0
```
```
cd $HOME/work
mkdir -p build_newlib
cd build_newlib
../newlib-cygwin/newlib/configure CC=clang CFLAGS="-nostdlibinc -O2" --target=x86_64-elf --prefix=$HOME/x86_64-elf --disable-multilib
make -j 4
make install
```
#### libc++abi
```
git clone --depth 1 --branch release/10.x https://github.com/llvm/llvm-project.git
mkdir build_libcxxabi
cd build_libcxxabi
cmake -G "Unix Makefiles" \
  -DCMAKE_INSTALL_PREFIX=$HOME/x86_64-elf \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_CXX_FLAGS="-I$HOME/x86_64-elf/include -nostdlibinc -O2 -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS -D_LIBCPP_HAS_NO_THREADS" \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_C_FLAGS="-I$HOME/x86_64-elf/include -nostdlibinc -O2 -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS -D_LIBCPP_HAS_NO_THREADS" \
  -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
  -DCMAKE_BUILD_TYPE=Release \
  -DLIBCXXABI_LIBCXX_INCLUDES="$HOME/work/llvm-project/libcxx/include" \
  -DLIBCXXABI_ENABLE_EXCEPTIONS=False \
  -DLIBCXXABI_ENABLE_THREADS=False \
  -DLIBCXXABI_TARGET_TRIPLE=x86_64-elf \
  -DLIBCXXABI_ENABLE_SHARED=False \
  -DLIBCXXABI_ENABLE_STATIC=True \
  $HOME/work/llvm-project/libcxxabi
```
#### libc++
```
git clone --depth 1 --branch release/10.x https://github.com/llvm/llvm-project.git
mkdir build_libcxx
cd build_libcxx
cmake -G "Unix Makefiles" \
  -DCMAKE_INSTALL_PREFIX=$HOME/x86_64-elf \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_CXX_FLAGS="-I$HOME/x86_64-elf/include -nostdlibinc -O2 -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS" \
  -DCMAKE_CXX_COMPILER_TARGET=x86_64-elf \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_C_FLAGS="-I$HOME/x86_64-elf/include -nostdlibinc -O2 -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS" \
  -DCMAKE_C_COMPILER_TARGET=x86_64-elf \
  -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
  -DCMAKE_BUILD_TYPE=Release \
  -DLIBCXX_CXX_ABI=libcxxabi \
  -DLIBCXX_CXX_ABI_INCLUDE_PATHS="$HOME/work/llvm-project/libcxxabi/include" \
  -DLIBCXX_CXX_ABI_LIBRARY_PATH="$HOME/x86_64-elf/lib" \
  -DLIBCXX_ENABLE_EXCEPTIONS=False \
  -DLIBCXX_ENABLE_FILESYSTEM=False \
  -DLIBCXX_ENABLE_MONOTONIC_CLOCK=False \
  -DLIBCXX_ENABLE_RTTI=False \
  -DLIBCXX_ENABLE_THREADS=False \
  -DLIBCXX_ENABLE_SHARED=False \
  -DLIBCXX_ENABLE_STATIC=True \
  $HOME/work/llvm-project/libcxx
```