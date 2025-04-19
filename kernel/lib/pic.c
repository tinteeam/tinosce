#include "pic.h"
#include "../drivers/ports.h"

// PIC ports for commands and data
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

// Initialization Command for both PICs
#define PIC_INIT_CMD 0x11
#define PIC_EOI_CMD  0x20  // End of Interrupt command

void init_pic() {
    // Remap IRQs 0-15 to IDT entries 32-47
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0); // unmask IRQ1 here if you want
    outb(0xA1, 0x0);
}


// Acknowledge the interrupt from the PIC
void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI_CMD);  // Send EOI to PIC2
    }
    outb(PIC1_COMMAND, PIC_EOI_CMD);      // Send EOI to PIC1
}
