#include "mm/mmu.h"
#include "mm/pmm.h"

static uint64_t *get_or_alloc(uint64_t *table, uint64_t idx)
{
  if (table[idx] == 0)
  {
    table[idx] = (uint64_t)pmm_alloc() | TABLE_FLAGS;
  }
  return (uint64_t *)(table[idx] & ~0xFFFUL);
}

void map_page(uint64_t *pgd, uint64_t va, uint64_t pa, uint64_t flags)
{
  uint64_t *l1 = get_or_alloc(pgd, (va >> PGD_SHIFT) & INDEX_MASK);
  uint64_t *l2 = get_or_alloc(l1, (va >> PUD_SHIFT) & INDEX_MASK);
  uint64_t *l3 = get_or_alloc(l2, (va >> PMD_SHIFT) & INDEX_MASK);

  l3[(va >> PTE_SHIFT) & INDEX_MASK] = pa | flags;
}

void mmu_init(void)
{
    uint64_t *pgd = (uint64_t *)pmm_alloc();

    uint64_t mair = (0xFFUL << 0) | (0x04UL << 8);
    asm volatile("msr mair_el1, %0" :: "r"(mair));

    uint64_t tcr = (16UL << 0)  |
                   (0UL  << 14) |
                   (1UL  << 8)  |
                   (1UL  << 10) |
                   (3UL  << 12);
    asm volatile("msr tcr_el1, %0" :: "r"(tcr));

    for (uint64_t addr = 0x00000000UL; addr < 0x3F000000UL; addr += PAGE_SIZE)
        map_page(pgd, addr, addr, PAGE_FLAGS_NORMAL);

    for (uint64_t addr = 0x3F000000UL; addr < 0x40000000UL; addr += PAGE_SIZE)
        map_page(pgd, addr, addr, PAGE_FLAGS_DEVICE);

    asm volatile("msr ttbr0_el1, %0" :: "r"(pgd));

    asm volatile("tlbi vmalle1is");
    asm volatile("dsb sy");
    asm volatile("isb");

    uint64_t sctlr;
    asm volatile("mrs %0, sctlr_el1" : "=r"(sctlr));
    sctlr |= (1UL << 0) | (1UL << 2) | (1UL << 12);
    asm volatile("msr sctlr_el1, %0" :: "r"(sctlr));
    asm volatile("isb");
}
