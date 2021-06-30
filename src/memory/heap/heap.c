#include "heap.h"
#include "kernel.h"
#include "status.h"
#include "memory/memory.h"
#include <stdbool.h>

// Ensure that user's block size calculation is correct.
static int heap_validate_table(void* ptr, void* end, struct heap_table* table)
{
    int response = 0;

    size_t table_size = (size_t)(end - ptr);
    size_t total_blocks = table_size / PEACHOS_HEAP_BLOCK_SIZE;

    if(table->total != total_blocks)
    {
        response = -EINVARG;
        goto out;
    }


    out: return response;
}

// Make sure pointers are aligned to memory blocks.
static bool heap_validate_alignment(void* ptr)
{
    return ((unsigned int)ptr % PEACHOS_HEAP_BLOCK_SIZE) == 0;
}

///////////////////////////////////////////////////////////
// Create an allocation in the provided heap and table.
///////////////////////////////////////////////////////////
// Return 0 for no error, negative number for error.
int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table)
{
    int response = PEACHOS_ALL_OK;

    if(!heap_validate_alignment(ptr) || !heap_validate_alignment(end))
    {
        response = -EINVARG;
        goto out;
    }

    memset(heap, 0, sizeof(struct heap));
    heap->saddr = ptr;
    heap->table = table;

    response = heap_validate_table(ptr, end, table);
    if(response < 0)
    {
        goto out;
    }

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

    out: return response;
}

void* heap_malloc(size_t size)
{
    return 0;
}

void heap_free(void *ptr)
{
    return 0;

}