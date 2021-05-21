#pragma once

#include <stddef.h>
#include <stdarg.h>

#define min(a, b) a > b ? b : a
#define max(a, b) a > b ? a : b

void operator delete(void *obj, void *buf);
void operator delete(void *obj);