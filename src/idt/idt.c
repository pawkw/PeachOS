#include "idt.h"
#include "config.h"
#include "memory/memory.h"

struct idt_desc idt_descriptors[PEACHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

void idt_zero() {

}

void idt_set(int interrupt_number, void* address) {
    struct idt_desc* desc = &idt_descriptors[interrupt_number];
    desc->offset_1 = (uint32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0;  // Surprise !!!
    desc->type_att = 0xee;
    desc->offset_2 = (uint32_t) address >> 16;
}

void idt_init() {
    // Zero out idt descriptors
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
    idtr_descriptor.base = idt_descriptors;

    // Set an interrupt
    idt_set(0, idt_zero); // example only

}

