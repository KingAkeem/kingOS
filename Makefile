CC=i686-elf-gcc
ASM=i686-elf-as
CFLAGS=-ffreestanding -O2
default: all

all: bootloader kernel linker start


bootloader: 
	$(ASM) bootloader.s -o bootloader.o	
kernel:
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o -std=gnu99 -Wall -Wextra
linker:
	$(CC) $(CFLAGS)  -T linker.ld -o kingos.bin -nostdlib bootloader.o kernel.o -lgcc 
start:
	qemu-system-i386 -cdrom kingos.iso
