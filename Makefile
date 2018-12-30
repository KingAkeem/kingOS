CC=i686-elf-gcc
CFLAGS=-ffreestanding -O2
kernel:
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o -std=gnu99 -Wall -Wextra
linker:
	$(CC) $(CFLAGS)  -T linker.ld -o kingos.bin -nostdlib bootloader.o kernel.o -lgcc 
start:
	qemu-system-i386 -cdrom kingos.iso
