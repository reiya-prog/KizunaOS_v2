.intel_syntax noprefix

.global io_hlt
io_hlt:
    hlt
    jmp io_hlt
    ret

.global io_out32
io_out32:
    mov dx, di
    mov eax, esi
    out dx, eax
    ret

.global io_in32
io_in32:
    mov dx, di
    in eax, dx
    ret