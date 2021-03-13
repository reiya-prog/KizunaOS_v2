# Kizuna OS
ぼちぼちがんばりたい
## Requirements
Windows wsl2(Ubuntu 20.04.1 LTS)もしくはMac OSを想定しています。<br>
wsl2のQEMUはCUI実行のみなので実行自体はWindows上でやっていたりしてます。
### common
#### clang++
- Ubuntu
```
sudo apt install clang -y
```
- Mac OS
おそらくデフォルトで入っています
#### lld
- Ubuntu
```
sudo apt install lld-6.0
```
- Mac OS
```
brew install llvm
# 必要に応じてPATHを通す
LLVM_PREFIX=`brew --prefix llvm`
export PATH=$PATH:$LLVM_PREFIX/bin
```
### standard library
#### newlib
```
mkdir -p $HOME/work
cd $HOME/work
git clone git://sourceware.org/git/newlib-cygwin.git
cd newlib-cygwin
git checkout newlib-2_5_0
cd $HOME/work
mkdir -p build_newlib
cd build_newlib
../newlib-cygwin/newlib/configure CC=clang CFLAGS="-nostdlibinc -O2" --target=x86_64-elf --prefix=$HOME/x86_64-elf --disable-multilib

make -j 4
make install
```
#### libc++abi
```
cd $HOME/work
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

make -j4
make install
```
#### libc++
```
cd $HOME/work
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

make -j4
make install
```