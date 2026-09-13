#pragma once

#include "types.h"

struct HeapBlock
{
  uint64_t size;                /* payload size in bytes (excludes header) */
  uint64_t is_free;             /* 1 = free, 0 = allocated */
  struct HeapBlock *next_block; /* next block in the linked list */
  struct HeapBlock *prev_block; /* previous block for backward coalescing */
};

void heap_init(void);
void *kmalloc(uint64_t size);
void kfree(void *ptr);
