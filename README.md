# kingOS

kingOS is a tiny 32-bit hobby operating system for ix86 CPUs. It boots through
GRUB using the Multiboot specification and currently provides a freestanding C
kernel with VGA text-mode output.

## Features

- Multiboot-compatible boot path.
- 16 KiB protected-mode stack set up before entering C.
- VGA text terminal with colored output, wrapping, scrolling, tabs, carriage
  returns, backspace handling, and decimal number output.
- ISO build target suitable for QEMU.

## Requirements

Install these tools before building:

- `i686-elf-gcc`
- `i686-elf-as`
- `grub-mkrescue`
- `grub-file` for `make check`
- `qemu-system-i386` for `make run`

On Debian or Ubuntu systems, `grub-mkrescue`, `grub-file`, and QEMU are commonly
available through `grub-pc-bin`, `xorriso`, and `qemu-system-x86`. The
cross-compiler toolchain is usually built separately so the kernel does not link
against host operating-system libraries.

## Build

```sh
make
```

This creates `kingos.iso`.

To validate the kernel image:

```sh
make check
```

To run in QEMU:

```sh
make run
```

To remove generated files:

```sh
make clean
```

## Project Structure

- `bootloader.s`: Multiboot header and early x86 setup.
- `kernel.c`: Freestanding kernel entry point and VGA terminal.
- `linker.ld`: Kernel linker script.
- `boot/grub/grub.cfg`: GRUB menu entry.
- `Makefile`: Build, validation, run, and clean targets.

## Development Notes

The checked-in source is intentionally small and freestanding. Keep kernel code
independent from host libc behavior, and prefer simple hardware-facing helpers
over abstractions until more subsystems exist.
