#include <error.h>
#undef errno
extern int errno;

#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>

caddr_t sbrk(int incr)
{
    errno = ENOMEM;
    return 0;
}

int close(int file)
{
    return -1;
}

int fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int isatty(int file)
{
    return 1;
}

int lseek(int file, int ptr, int dir)
{
    return 0;
}

int read(int file, char *ptr, int len)
{
    return 0;
}

int write(int file, char *ptr, int len)
{
    return 0;
}