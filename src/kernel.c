#include "kernel.h"

uint16_t* video_mem = 0;

int len(char string[]) {
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

void display(uint16_t* screen, char string[] , int line, int col, char fg, char bg) {
    int length = len(string);
    int start = line * VGA_WIDTH + col;
    for(int i = 0; i < length; i++) {
        screen[start + i] = terminal_make_char(string[i], terminal_make_colour(fg, bg));
    }
}

void terminal_initialize() {
    video_mem = (uint16_t*)0xb8000;
    uint16_t blank = terminal_make_char(' ', terminal_make_colour(8, 15));
    for(int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            video_mem[(y*VGA_WIDTH)+x] = blank;
        }
        
    }

}

void kernel_main() {
    char string[] = "Welcome to Agree JOS.\0";
    
    terminal_initialize();
    display(video_mem, string, 10, 40 - (len(string)>>1), 8, 15);
}
