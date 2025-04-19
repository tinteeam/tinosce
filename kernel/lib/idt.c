#include "idt.h"
#include "ktypes.h"
#include "kprint.h"
#include <stddef.h>

#define IDT_ENTRIES 256

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;        // Kernel segment selector
    uint8_t always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));



struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

extern struct idt_ptr idtp;



extern void idt_load();     // Implemented in ASM
extern void isr33();        // Your keyboard interrupt stub

isr_t interrupt_handlers[IDT_ENTRIES];

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void isr_handler(uint8_t int_no) {
    if (interrupt_handlers[int_no]) {
        interrupt_handlers[int_no]();
    }
}

static void idt_set_entry(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0  = 0;
    idt[num].flags    = flags;
}

void init_idt() {
    // Set IDT pointer (base address and limit)
    idtp.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idtp.base  = (uint32_t)&idt;

    // Clear the IDT
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_entry(i, 0, 0, 0);  // Clear all entries (invalid interrupt gates)
    }

    // Set up keyboard interrupt handler (IRQ1 -> INT 33)
    idt_set_entry(33, (uint32_t)isr33, 0x08, 0x8E);  // Register the keyboard interrupt handler

    // Load the IDT
    idt_load();  // This should load the IDT into the IDTR register
}

