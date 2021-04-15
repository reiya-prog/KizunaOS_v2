.intel_syntax noprefix

.global io_hlt
io_hlt:
    hlt
    jmp io_hlt
    ret
