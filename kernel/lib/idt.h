#ifndef IDT_H
#define IDT_H

#include "ktypes.h"



typedef void (*isr_t)(void);


struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
    /* data */
} __attribute__((packed));




extern void register_interrupt_handler(uint8_t n, isr_t handler);
void init_idt();

#endif
