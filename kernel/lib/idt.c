#include "idt.h"
#include "ktypes.h"

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#define IDT_SIZE 256
struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

extern void isr33(); // from isr.asm

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

extern void idt_flush(uint32_t); // implemented in assembly

void init_idt() {
    idtp.limit = sizeof(struct idt_entry) * IDT_SIZE - 1;
    idtp.base  = (uint32_t)&idt;

    idt_set_gate(33, (uint32_t)isr33, 0x08, 0x8E); // IRQ1

    idt_flush((uint32_t)&idtp);
}
