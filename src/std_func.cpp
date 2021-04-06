#include "main.h"

void *operator new(size_t size, void *buf)
{
    return buf;
}

void operator delete(void *obj, void *buf) noexcept
{
}

void operator delete(void *obj) noexcept
{
}

void memset(void *dst, int value, int size)
{
    const unsigned char uc = value;
    unsigned char *p = reinterpret_cast<unsigned char *>(dst);
    while (size-- > 0)
        *p++ = uc;
}
