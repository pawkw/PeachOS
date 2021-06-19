#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// Iterrupt descriptor table.
// https://wiki.osdev.org/Interrupt_Descriptor_Table
struct idt_desc
{
    uint16_t offset_1; // Offset bits 0-15
    uint16_t selector; // Selector from GDT
    uint8_t zero;      // Reserved
    uint8_t type_att;  // Descriptor type and attribute
    uint16_t offset_2; // Offset bits 16-31
} __attribute__((packed)); // Keep the compiler from rearranging the data.

struct idtr_desc
{
    uint16_t limit; // Size of descriptor table - 1
    uint32_t base;  // Base address of table
} __attribute__((packed));

void idt_init();

#endif