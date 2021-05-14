#include "kernel.h"
#include <stdint.h>
#include <stddef.h>

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
    return (bg << 3) | fg;
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
    
    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col += 1;
    if(terminal_col >= VGA_WIDTH) {
        terminal_col = 0;
        terminal_row += 1;
    }
}

void terminal_writestr(char string[], char colour) {
    int l = strlen(string);
    for(int i = 0; i < l; i++) {
        terminal_writechar(string[i], colour);
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
    char string[] = "Welcome to Agree JOS\nby Peter Weston";
    
    terminal_initialize();
    terminal_writestr(string, terminal_make_colour(DEFAULT_FG, DEFAULT_BG));
    // display(video_mem, string, 10, 40 - (len(string)>>1), 8, 15);
}
