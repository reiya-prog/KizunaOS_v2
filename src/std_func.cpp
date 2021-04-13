#include "std_func.h"

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