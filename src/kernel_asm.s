.intel_syntax noprefix

.global io_hlt
io_hlt:
    hlt
    jmp io_hlt
    ret

.global io_cli
io_cli:
    cli
    ret

.global io_sti
io_sti:
    sti
    ret

.global io_stihlt
io_stihlt:
    sti
    hlt
    ret

.global io_read_8
io_read_8:
    mov edx, ecx
    xor rax, rax
    in al, dx
    ret

.global io_read_32
io_read_32:
    mov edx, ecx
    xor rax, rax
    in eax, dx
    ret

.global io_write_8
io_write_8:
    mov eax, edx
    mov edx, ecx
    out dx, al
    ret

.global io_write_32
io_write_32:
    mov eax, edx
    mov edx, ecx
    out dx, eax
    ret

.global default_handler
default_handler:
    hlt
    jmp default_handler

.global int21_handler
int21_handler:
    push rdi
    mov rdi, 0x21
    call handler_wrapper
    pop rdi
    iretq

.global handler_wrapper
handler_wrapper:
    push rax
    push rcx
    push rdx
    push rbx
    push rbp
    push rsi
    call do_default_handler
    pop rsi
    pop rbp
    pop rbx
    pop rdx
    pop rcx
    pop rax
    ret

.global load_gdt
load_gdt:
    lgdt [rcx]
    ret

.global load_idt
load_idt:
    lidt [rcx]
    ret

.global write_ss_selector
write_ss_selector:
    mov ss, cx
    ret

.global write_ds_selector
write_ds_selector:
    mov ds, cx
    ret

.global write_cs_selector
write_cs_selector:
    push rbp
    mov rbp, rsp
    lea rax, [rip + write_cs_end]
    push cx
    push rax
    ljmp [rax]
write_cs_end:
    add rsp, 8 + 2
    ret
