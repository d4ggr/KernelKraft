#include "mm/heap.h"
#include "drivers/uart.h"
#include "mm/pmm.h"
#include "types.h"

#define PAGE_WIDTH 4096

static struct HeapBlock *heap_start = NULL;

void heap_init(void)
{
  heap_start = pmm_alloc();

  heap_start->size = PAGE_WIDTH - sizeof(struct HeapBlock);
  heap_start->is_free = 1;
  heap_start->next_block = NULL;
  heap_start->prev_block = NULL;

  uart_puts("Heap is initialized\n");
  uart_puts("Heap start address : ");
  uart_hex((uint64_t)heap_start);
  uart_puts("\n");
}

void *kmalloc(uint64_t size)
{
  size = (size + 7) & ~7;

  struct HeapBlock *curr_block = heap_start;
  struct HeapBlock *prev_block = NULL;

  while (curr_block != NULL)
  {
    if (curr_block->is_free == 1 && curr_block->size >= size)
    {
      // split only if leftover can hold a header + minimum 8 bytes payload
      if (curr_block->size >= size + sizeof(struct HeapBlock) + 8)
      {
        struct HeapBlock *new_block =
            (struct HeapBlock *)((char *)curr_block + sizeof(struct HeapBlock) +
                                 size);

        new_block->size = curr_block->size - size - sizeof(struct HeapBlock);
        new_block->is_free = 1;
        new_block->next_block = curr_block->next_block;
        new_block->prev_block = curr_block;

        if (curr_block->next_block != NULL)
          curr_block->next_block->prev_block = new_block;

        curr_block->size = size;
        curr_block->next_block = new_block;
      }

      curr_block->is_free = 0;
      return (void *)((char *)curr_block + sizeof(struct HeapBlock));
    }

    prev_block = curr_block;
    curr_block = curr_block->next_block;
  }

  // no free block found — request a new page from PMM and attach to list tail
  struct HeapBlock *new_heap_arena = pmm_alloc();
  if (new_heap_arena == NULL)
  {
    uart_puts("kmalloc: out of memory\n");
    return NULL;
  }

  new_heap_arena->size = PAGE_WIDTH - sizeof(struct HeapBlock);
  new_heap_arena->is_free = 1;
  new_heap_arena->next_block = NULL;
  new_heap_arena->prev_block = prev_block;
  prev_block->next_block = new_heap_arena;

  return kmalloc(size);
}

void kfree(void *ptr)
{
  struct HeapBlock *block =
      (struct HeapBlock *)((char *)ptr - sizeof(struct HeapBlock));

  block->is_free = 1;

  // coalesce backward — merge into free predecessor if one exists
  while (block->prev_block != NULL && block->prev_block->is_free)
  {
    struct HeapBlock *prev = block->prev_block;
    prev->size += sizeof(struct HeapBlock) + block->size;
    prev->next_block = block->next_block;
    if (block->next_block != NULL)
      block->next_block->prev_block = prev;
    block = prev;
  }

  // coalesce forward — absorb any consecutive free successors
  while (block->next_block != NULL && block->next_block->is_free)
  {
    struct HeapBlock *next = block->next_block;
    block->size += sizeof(struct HeapBlock) + next->size;
    block->next_block = next->next_block;
    if (next->next_block != NULL)
      next->next_block->prev_block = block;
  }
}
