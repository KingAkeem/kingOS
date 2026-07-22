#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into issues."
#endif

/* This only works for 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This kernel will only compile with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fore, enum vga_color back) {
    return fore | back << 4;
}

static inline uint16_t vga_entry(unsigned char c, uint8_t color) {
    return (uint16_t) c | (uint16_t) color << 8;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define TAB_WIDTH 4

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static volatile uint16_t* terminal_buffer;

static void terminal_clear_row(size_t row) {
    for (size_t column = 0; column < VGA_WIDTH; column++) {
        terminal_buffer[row * VGA_WIDTH + column] = vga_entry(' ', terminal_color);
    }
}

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;

    for (size_t row = 0; row < VGA_HEIGHT; row++) {
        terminal_clear_row(row);
    }
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

static void terminal_scroll(void) {
    for (size_t row = 1; row < VGA_HEIGHT; row++) {
        for (size_t column = 0; column < VGA_WIDTH; column++) {
            const size_t from = row * VGA_WIDTH + column;
            const size_t to = (row - 1) * VGA_WIDTH + column;
            terminal_buffer[to] = terminal_buffer[from];
        }
    }

    terminal_clear_row(VGA_HEIGHT - 1);
    terminal_row = VGA_HEIGHT - 1;
    terminal_column = 0;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

static void terminal_newline(void) {
    terminal_column = 0;

    if (terminal_row + 1 >= VGA_HEIGHT) {
        terminal_scroll();
    } else {
        terminal_row++;
    }
}

void terminal_putchar(char c) {
    switch (c) {
    case '\n':
        terminal_newline();
        return;
    case '\r':
        terminal_column = 0;
        return;
    case '\t':
        do {
            terminal_putchar(' ');
        } while (terminal_column % TAB_WIDTH != 0);
        return;
    case '\b':
        if (terminal_column > 0) {
            terminal_column--;
            terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
        }
        return;
    default:
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

        if (++terminal_column >= VGA_WIDTH) {
            terminal_newline();
        }
        return;
    }
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void terminal_write_dec(size_t value) {
    char digits[20];
    size_t length = 0;

    if (value == 0) {
        terminal_putchar('0');
        return;
    }

    while (value > 0 && length < sizeof(digits)) {
        digits[length++] = (char) ('0' + value % 10);
        value /= 10;
    }

    while (length > 0) {
        terminal_putchar(digits[--length]);
    }
}

void terminal_writestring_colored(const char* data, enum vga_color foreground, enum vga_color background) {
    const uint8_t previous_color = terminal_color;
    terminal_setcolor(vga_entry_color(foreground, background));
    terminal_writestring(data);
    terminal_setcolor(previous_color);
}

static void terminal_write_header(void) {
    terminal_writestring_colored("kingOS", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_writestring(" - tiny ix86 kernel\n");
    terminal_writestring_colored("OK", VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREEN);
    terminal_writestring(" VGA terminal initialized\n\n");
}

void kernel_main(void) {
    /* Initialize terminal interface. */
    terminal_initialize();
    terminal_write_header();
    terminal_writestring("Hello, kernel world!\n");
    terminal_writestring("Features: wrapping, scrolling, tabs\tand colored output.\n\n");

    for (size_t line = 1; line <= 30; line++) {
        terminal_writestring("scroll test line ");
        terminal_write_dec(line);
        terminal_putchar('\n');
    }

    terminal_writestring_colored("System halted.", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
}
