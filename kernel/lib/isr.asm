[BITS 32]
global isr33
extern isr_handler

isr33:
    cli
    pusha
    mov al, 33
    push eax
    call isr_handler
    add esp, 4
    popa
    mov al, 0x20
    out 0x20, al
    sti
    iret