#include "kernel.h"

int len(char string[]) {
    int count = 0;
    while(string[count] != '\0') ++count;
    return count;
}

void display(char* screen, char string[] , int line, int col, int fg, int bg) {
    int length = len(string);
    int start = line * 80 + (col<<1);
    for(int i = 0; i < length; i++) {
        screen[start + (i<<1)] = string[i];
        screen[start + (i<<1) + 1] = (bg << 3) + fg;
    }
}

void cls(char* screen, char character, int colour) {
    for(int i = 0; i < 2000; i++) {
        screen[i<<1] = character;
        screen[(i<<1)+1] = (colour<<4);

    }
}

void kernel_main() {
    char* video_mem = (char*)0xb8000;
    char string[] = "Welcome to Agree JOS.\0";
    
    cls(video_mem, ' ', 1);
    display(video_mem, string, 10, 40 - (len(string)>>1), 15, 1);
}
