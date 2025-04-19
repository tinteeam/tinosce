#include "../../lib/ktypes.h"
#include "../../lib/kprint.h"
#include "../ports.h"
#include "keyboard.h"
#include "../../lib/idt.h"
#include "../../lib/pic.h"  // Include PIC initialization header

extern void register_interrupt_handler(uint8_t n, void (*handler)()); // You should provide this function for registering handlers

#define KEYBOARD_DATA_PORT 0x60

static char keymap[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ', 0,
    // ...
};

static volatile char last_char = 0;

// Interrupt handler for keyboard
void keyboard_handler() {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    if (!(scancode & 0x80)) {  // If the scancode is not a "release" code
        char key = keymap[scancode];
        if (key) {
            last_char = key;
            kprint("Key pressed!\n");
        }
    }

     pic_send_eoi(1);
}

// Get a character from the keyboard input (blocking)
char getchar() {
    while (!last_char);  // Wait until a key is pressed
    char c = last_char;
    last_char = 0;  // Reset for next character
    return c;
}

// Initialize the keyboard and PIC
void keyboard_init() {
    kprint("Initializing keyboard...\n");

    // Initialize the PIC (unmask IRQ1 for keyboard)
    init_pic();

   

    // Register the keyboard interrupt handler (IRQ1)
    register_interrupt_handler(33, keyboard_handler);  // IRQ1 corresponds to interrupt 33
}
