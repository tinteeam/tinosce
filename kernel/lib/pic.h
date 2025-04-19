#ifndef PIC_H
#define PIC_H

#include "ktypes.h"

void init_pic();             // Initialize the PIC
void pic_send_eoi(uint8_t irq); // Send End-of-Interrupt signal to PIC

#endif
