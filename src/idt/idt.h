#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_desc
{
    uint16_t offset_1; // Offset bits 0-15
    uint16_t selector; // Selector from GDT
    uint8_t zero;      // Reserved
    uint8_t type_att;  // Descriptor type
    uint16_t offset_2; // Offset bits 16-31
} __attribute__((packed));

struct idtr_desc
{
    uint16_t limit; // Size of descriptor table - 1
    uint32_t base;  // Base address of table
} __attribute__((packed));

#endif