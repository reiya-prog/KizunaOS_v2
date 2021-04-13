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


int dec_to_asc(char *str, int dec)
{
    int len = 0, len_buf;
    int buf[10];
    while (1)
    {
        buf[len++] = dec % 10;
        if (dec < 10)
            break;
        dec /= 10;
    }
    len_buf = len;
    while (len)
    {
        *(str++) = buf[--len] + 0x30;
    }
    return len_buf;
}

int hex_to_asc(char *str, int dec)
{
    int len = 0, len_buf;
    int buf[10];
    while (1)
    {
        buf[len++] = dec % 16;
        if (dec < 16)
            break;
        dec /= 16;
    }
    len_buf = len;
    while (len)
    {
        len--;
        *(str++) = (buf[len] < 10) ? (buf[len] + 0x30) : (buf[len] - 9 + 0x60);
    }
    return len_buf;
}

void sprintf(char *str, char *format, ...)
{
    va_list list;
    int i, len;
    va_start(list, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'd':
                len = dec_to_asc(str, va_arg(list, int));
                break;
            case 'x':
                len = hex_to_asc(str, va_arg(list, int));
                break;
            }
            str += len;
            format++;
        }
        else
        {
            *(str++) = *(format++);
        }
    }
    *str = 0x00;
    va_end(list);
}