#pragma once

#include "types.h"

#define PGD_SHIFT 39
#define PUD_SHIFT 30
#define PMD_SHIFT 21
#define PTE_SHIFT 12

#define TABLE_SIZE 512
#define INDEX_MASK 0x1FFUL

#define TD_VALID (1UL << 0)
#define TD_TABLE (1UL << 1)
#define TD_PAGE (1UL << 1)
#define TD_ATTR_INDX(n) ((uint64_t)(n) << 2)
#define TD_AP_RW (0UL << 6)
#define TD_INNER_SHARE (3UL << 8)
#define TD_AF (1UL << 10)

#define PAGE_FLAGS_NORMAL                                                      \
  (TD_VALID | TD_PAGE | TD_ATTR_INDX(0) | TD_AF | TD_INNER_SHARE)
#define PAGE_FLAGS_DEVICE (TD_VALID | TD_PAGE | TD_ATTR_INDX(1) | TD_AF)
#define TABLE_FLAGS (TD_VALID | TD_TABLE)

void map_page(uint64_t *pgd, uint64_t va, uint64_t pa, uint64_t flags);
void mmu_init(void);
