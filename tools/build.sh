#!/bin/bash

# Directories
BUILD_DIR="build"
ISO_DIR="dist/iso"
GRUB_DIR="$ISO_DIR/boot/grub"
KERNEL_BIN="$BUILD_DIR/kernel.bin"
ISO_FILE="dist/os.iso"

# Source files
ASM_SOURCES=(
  "kernel/entry.S"
  "kernel/lib/idt_load.asm"
  "kernel/lib/isr.asm"
  "kernel/lib/idt_flush.asm"
)
C_SOURCES=(
  "kernel/main.c"
  "kernel/lib/kprint.c"
  "kernel/lib/stdio.c"
  "kernel/console/console.c"
  "kernel/drivers/ports.c"
  "kernel/drivers/keyboard/keyboard.c"
)
LIB_DIR="kernel/lib"

# Object files
ASM_OBJECTS=()
C_OBJECTS=()

# Compiler flags
GCC_FLAGS="-m32 -ffreestanding -I$LIB_DIR"
NASM_FLAGS="-f elf32"

# Create build directory if not exists
mkdir -p $BUILD_DIR
mkdir -p dist

# Function to handle errors
handle_error() {
  echo "Build failed with errors!"
  exit 1
}

# Function to compile assembly files
compile_asm() {
  for src in "${ASM_SOURCES[@]}"; do
    obj_file="$BUILD_DIR/$(basename $src .asm).o"
    nasm $NASM_FLAGS $src -o $obj_file || handle_error
    ASM_OBJECTS+=($obj_file)
  done
}

# Function to compile C files
compile_c() {
  for src in "${C_SOURCES[@]}"; do
    obj_file="$BUILD_DIR/$(basename $src .c).o"
    gcc $GCC_FLAGS -c $src -o $obj_file || handle_error
    C_OBJECTS+=($obj_file)
  done
}

# Function to link object files and create the kernel binary
link_kernel() {
  # Link all object files (C and ASM) into a kernel binary
  ld -m elf_i386 -T kernel/linker.ld -o $KERNEL_BIN "${ASM_OBJECTS[@]}" "${C_OBJECTS[@]}" || handle_error
}

# Function to create ISO image using GRUB
create_iso() {
  mkdir -p $GRUB_DIR
  cp $KERNEL_BIN $ISO_DIR/boot/tinosce.bin
  cp boot/grub.cfg $GRUB_DIR/grub.cfg
  grub-mkrescue -o $ISO_FILE $ISO_DIR || handle_error
}

# Clean previous build files
clean() {
  rm -rf $BUILD_DIR
  rm -rf dist
}

# Build steps
case $1 in
  clean)
    clean
    ;;
  build)
    echo "Building OS..."
    compile_asm
    compile_c
    link_kernel
    create_iso
    echo "Build complete!"
    ;;
  run)
    echo "Running OS in QEMU..."
    qemu-system-i386 -cdrom $ISO_FILE -boot d || handle_error
    ;;
  *)
    echo "Usage: $0 {clean|build|run}"
    exit 1
    ;;
esac
