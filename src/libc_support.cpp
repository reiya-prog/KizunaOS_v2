#include "libc_support.h"

extern "C"
{
    caddr_t sbrk(int incr)
    {
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

    int posix_memalign(void **memptr, size_t alignment, size_t size)
    {
        return 0;
    }

    void _exit(int status)
    {
    }

    int kill(pid_t pid, int sig)
    {
        return 0;
    }

    pid_t getpid(void)
    {
        return 0;
    }
}