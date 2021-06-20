#ifndef KERNEL_H
#define KERNEL_H

// Colours used by terminal
enum {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    YELLOW,
    WHITE,
    GRAY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    BRIGHT_YELLOW,
    BRIGHT_WHITE
};

#define DEFAULT_FG WHITE
#define DEFAULT_BG BLUE
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void kernel_main();

// Display a string on screen
void display(char string[]);

// Display a string with supplied colours
void display_colour(char string[], char fg_colour, char bg_colour);

#endif

