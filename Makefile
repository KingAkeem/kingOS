CC=i686-elf-gcc
ASM=i686-elf-as
CFLAGS=-ffreestanding -O2
default: all

all: bootloader kernel linker iso start

clean:
	rm bootloader.o kernel.o boot/kingos.bin kingos.iso 
bootloader: 
	$(ASM) bootloader.s -o bootloader.o	
kernel:
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o -std=gnu99 -Wall -Wextra
linker:
	$(CC) $(CFLAGS) -T linker.ld -o boot/kingos.bin -nostdlib bootloader.o kernel.o -lgcc 
iso:
	grub-mkrescue -o kingos.iso .
start:
	qemu-system-i386 -cdrom kingos.iso
