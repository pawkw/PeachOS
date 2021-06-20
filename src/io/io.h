#ifndef io_h
#define io_h

// In single byte.
// Returns the result of an assembler 'in' instruction.
unsigned char insb(unsigned short port);

// In single word.
// Returns the result of an assembler 'in' instruction.
unsigned short insw(unsigned short port);


// Out single byte.
// Returns the result of an assembler 'out' instruction.
void outb(unsigned short port, unsigned char byte);

// Out single word.
// Returns the result of an assembler 'out' instruction.
void outw(unsigned short port, unsigned short word);

#endif