CC = i686-elf-gcc
AS = i686-elf-as
QEMU = qemu-system-i386
GRUB_MKRESCUE = grub-mkrescue
GRUB_FILE = grub-file

CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Werror
LDFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib

KERNEL := boot/kingos.bin
ISO := kingos.iso
OBJECTS := bootloader.o kernel.o

.PHONY: all clean iso run check

all: $(ISO)

$(ISO): $(KERNEL) boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ .

$(KERNEL): $(OBJECTS) linker.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) -lgcc

bootloader.o: bootloader.s
	$(AS) $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

iso: $(ISO)

check: $(KERNEL)
	$(GRUB_FILE) --is-x86-multiboot $(KERNEL)

run: $(ISO)
	$(QEMU) -cdrom $(ISO)

clean:
	rm -f $(OBJECTS) $(KERNEL) $(ISO)
