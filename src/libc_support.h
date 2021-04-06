#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>

extern "C"
{
    caddr_t sbrk(int incr);

    int close(int file);

    int fstat(int file, struct stat *st);

    int isatty(int file);

    int lseek(int file, int ptr, int dir);

    int read(int file, char *ptr, int len);

    int write(int file, char *ptr, int len);

    int posix_memalign(void **memptr, size_t alignment, size_t size);

    void _exit(int status);

    int kill(pid_t pid, int sig);

    pid_t getpid(void);
}