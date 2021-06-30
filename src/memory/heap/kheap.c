#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "kernel.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init()
{
    int total_table_entries = PEACHOS_HEAP_SIZE_BYTES / PEACHOS_HEAP_SIZE_BYTES;
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*) (PEACHOS_HEAP_TABLE_ADDRESS);   
    kernel_heap_table.total = total_table_entries;

    void* end = (void*)(PEACHOS_HEAP_ADDRESS + PEACHOS_HEAP_SIZE_BYTES);
    int response = heap_create(&kernel_heap, (void*)(PEACHOS_HEAP_ADDRESS), end, &kernel_heap_table);
    if (response < 0)
    {
        // TODO Change this to panic.
        display_colour("Failed to create kernel heap.\n", RED, DEFAULT_BG);
    }   
}