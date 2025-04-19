#include "lib/multiboot.h"

//include console
#include "console/console.h"


//includ standard lib stuff
#include "lib/stdio.h"
#include "lib/kprint.h"
#include "lib/idt.h"


//include drivers to init!
//DO NOT REMOVE THIS SECTION!
#include "drivers/keyboard/keyboard.h"



//enabe interrupts
void enable_interrupts() {
    asm volatile ("sti");
}


__attribute__((section(".multiboot")))
struct multiboot_header_t mboot_header = {
    .magic = MULTIBOOT_MAGIC,
    .flags = MULTIBOOT_FLAGS,
    .checksum = -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)
};

void _main(struct multiboot_info_t *mboot_info, uint32_t mboot_magic) {
    
    //enable_interrupts();
    
    init_idt();
	keyboard_init();
    console_init();
    console_run();
	//kprint("test");
}
