#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"
#include "io/io.h"

struct idt_desc idt_descriptors[PEACHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void idt_load(struct idtr_desc* ptr);
extern void int21h();
extern void no_interrupt();

void int21h_handler()
{
    display_colour("Key pressed.\n", GREEN, BLUE);
    int_acknowledge;
}

void no_interrupt_handler()
{
    int_acknowledge;
}

void idt_zero() {
    display_colour("Divide by zero error.", RED, DEFAULT_BG);
}

void idt_set(int interrupt_number, void* address) {
    struct idt_desc* desc = &idt_descriptors[interrupt_number];
    desc->offset_1 = (uint32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0;  // Surprise !!!
    desc->type_att = 0xee; // See after subroutine.
    desc->offset_2 = (uint32_t) address >> 16;
}

// Type Attribute
/* 4 bits Type: e = 32 bit interrupt gate
   1 bit P    : 1 = present
   2 bits DPL : 11 = ring 3 privilege level
   1 bit S    : 0 = storage segment. Set to zero for interrupt and trap gates */

void idt_init() {
    // Zero out idt descriptors
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
    idtr_descriptor.base = (uint32_t) idt_descriptors;

    // Set temporary handler
    for(int i = 0; i < PEACHOS_TOTAL_INTERRUPTS; i++)
    {
        idt_set(i, no_interrupt);
    }

    // Set an interrupt
    idt_set(0, idt_zero);

    // Set keyboard int
    idt_set(0x21, int21h);

    // Load descriptor table
    idt_load(&idtr_descriptor);
}

