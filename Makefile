BUILD     = build
KERNEL    = $(BUILD)/kernel.bin
GRUBDIR   = $(BUILD)/iso/boot/grub
ISODIR    = $(BUILD)/iso
ISO       = dist/os.iso

ASM_SRC   = kernel/entry.S
C_SRC     = kernel/main.c
KPRINT_SRC = kernel/lib/kprint.c
STDIO_SRC  = kernel/lib/stdio.c
LD_SCRIPT = kernel/linker.ld
LIB_DIR   = $(PWD)/kernel/lib
CONSOLE_SRC = kernel/console/console.c
PORTS_SRC = kernel/drivers/ports.c
KEYBOARD_SRC = kernel/drivers/keyboard/keyboard.c
IDT_SRC    = kernel/lib/idt.c  # Assuming you have a separate file for IDT setup
ISR_SRC    = kernel/lib/isr.asm  # Your ISR assembly code
IDT_FLUSH_SRC = kernel/lib/idt_flush.asm  # IDT flush
IDT_LD_SRC = kernel/lib/idt_load.asm 

OBJS = $(BUILD)/entry.o $(BUILD)/main.o $(BUILD)/kprint.o $(BUILD)/stdio.o \
       $(BUILD)/console.o $(BUILD)/ports.o $(BUILD)/keyboard.o $(BUILD)/idt.o \
		$(BUILD)/isr.o $(BUILD)/idt_flush.o $(BUILD)/pic.o $(BUILD)/idt_load

GRUB_CFG = boot/grub.cfg

.PHONY: all clean run

all: $(ISO)

# Kernel linking step
$(KERNEL): $(OBJS)
	ld -m elf_i386 -T $(LD_SCRIPT) -o $@ $(OBJS)

# Object file rules
$(BUILD)/entry.o: $(ASM_SRC)
	mkdir -p $(BUILD)
	nasm -f elf32 $< -o $@

$(BUILD)/main.o: $(C_SRC)
	gcc -m32 -ffreestanding -c $< -I$(LIB_DIR) -o $@

$(BUILD)/kprint.o: $(KPRINT_SRC)
	gcc -m32 -ffreestanding -c $< -I$(LIB_DIR) -o $@

$(BUILD)/stdio.o: $(STDIO_SRC)
	gcc -m32 -ffreestanding -c $< -I$(LIB_DIR) -o $@

$(BUILD)/console.o: $(CONSOLE_SRC)
	gcc -m32 -ffreestanding -c $< -I$(LIB_DIR) -o $@

$(BUILD)/ports.o: $(PORTS_SRC)
	gcc -m32 -ffreestanding -c $< -I$(LIB_DIR) -o $@

$(BUILD)/keyboard.o: $(KEYBOARD_SRC)
	gcc -m32 -ffreestanding -c $< -I$(LIB_DIR) -o $@

# IDT-related object rules
$(BUILD)/idt.o: $(IDT_SRC)
	gcc -m32 -ffreestanding -c $< -I$(LIB_DIR) -o $@

$(BUILD)/isr.o: $(ISR_SRC)
	nasm -f elf32 $< -o $@

$(BUILD)/idt_flush.o: $(IDT_FLUSH_SRC)
	nasm -f elf32 $< -o $@


$(BUILD)/idt_load.o: $(IDT_LD_SRC)
	nasm -f elf32 $< -o $@

# pic stuff

$(BUILD)/pic.o: kernel/lib/pic.c
	gcc -m32 -ffreestanding -c kernel/lib/pic.c -I$(LIB_DIR) -o $(BUILD)/pic.o

# Build ISO using grub-mkrescue
$(ISO): $(KERNEL) $(GRUB_CFG)
	mkdir -p $(GRUBDIR)
	cp $(KERNEL) $(ISODIR)/boot/tinosce.bin
	cp $(GRUB_CFG) $(GRUBDIR)/grub.cfg
	mkdir -p dist
	grub-mkrescue -o $@ $(ISODIR)

# Run ISO in QEMU
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -boot d

# Clean build artifacts
clean:
	rm -rf $(BUILD) dist