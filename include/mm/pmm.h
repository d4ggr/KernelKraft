#pragma once
#include "types.h"

#define PAGE_SIZE  4096
#define PAGE_SHIFT   12

void pmm_init(void);
void* pmm_alloc(void);
void pmm_free(void* ptr);
uint64_t pmm_get_free_page_count(void);
