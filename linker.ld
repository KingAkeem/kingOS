/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final kernel image. */
SECTIONS {
    /* Begin putting sections at 1 MiB, a conventional place for kernels to be loaded by the bootloader. */
    . = 1M;

    /* First put the multiboot header, as it is required to be very early in the image or the bootloader will not
       recognize the file format. Next we put the text section. */
    . = ALIGN(4K);
    .text : {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data. */
    . = ALIGN(4K);
    .rodata : {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    . = ALIGN(4K);
    .data : {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    . = ALIGN(4K);
    .bss : {
        *(.COMMON)
        *(.bss)
    }
}
