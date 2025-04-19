; idt.asm

global idtp

; The IDT pointer
idtp:
    ; First, we'll set up the IDT size and base address.
    ; The IDT is typically 256 entries, each 8 bytes.
    ; You can adjust the size if you're using a different number of entries.
    ; Format: [limit] [base address]
    ; 256 entries * 8 bytes per entry = 2048 bytes
    ; Limit (size of IDT) is 0x07FF (2047 entries, 0-indexed)
    ; Base is the address of the IDT.
    ; Assume the IDT starts at the address 0x0.

    ; Set up the limit (high byte of the size)
    dw 0x07FF       ; Limit = 0x07FF (2047, 256 entries of 8 bytes each)

    ; Set up the base address (low 16 bits and high 16 bits)
    dd 0x00000000    ; Base address (this will be updated when you initialize IDT)
