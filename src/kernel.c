#include <stdint.h>
#include <stddef.h>
#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

size_t strlen(char string[]) {
    int count = 0;
    while(string[count] != '\0') ++count;
    return count;
}

uint16_t terminal_make_char(char c, char colour) {
    return (colour << 8) | c;
}

char terminal_make_colour(char fg, char bg) {
    return (bg << 4) | fg;
}

void terminal_putchar(int x, int y, char c, char colour) {
    video_mem[y*VGA_WIDTH+x] = terminal_make_char(c, colour);
}

void terminal_writechar(char c, char colour) {
    if(c == '\n') {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }
    if(c == '\t')
    {
        terminal_row += 4;
        if(terminal_col >= VGA_WIDTH) {
            terminal_col = 0;
            terminal_row += 1;
        }
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col += 1;
    if(terminal_col >= VGA_WIDTH) {
        terminal_col = 0;
        terminal_row += 1;
    }
}

void display_colour(char string[], char fg_colour, char bg_colour) {
    int l = strlen(string);
    char colour = terminal_make_colour(fg_colour, bg_colour);
    for(int i = 0; i < l; i++) {
        terminal_writechar(string[i], colour);
    }
}

void display(char string[]) {
    int l = strlen(string);
    for(int i = 0; i < l; i++) {
        terminal_writechar(string[i], terminal_make_colour(DEFAULT_FG, DEFAULT_BG));
    }
}

void terminal_initialize() {
    video_mem = (uint16_t*)0xb8000;
    terminal_row = 0;
    terminal_col = 0;

    for(int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', terminal_make_colour(DEFAULT_FG, DEFAULT_BG));
        }
        
    }
}

void kernel_main() {
    char string[] = "Welcome to Agree JOS\nby Peter Weston\n";
    
    terminal_initialize();
    display(string);
    
    // Initialize the interrupt descriptor table.
    idt_init();


}
