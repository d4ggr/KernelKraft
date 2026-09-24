#include "drivers/timer.h"
#include "drivers/uart.h"
#include "irq.h"
#include "mm/heap.h"
#include "mm/mmu.h"
#include "mm/pmm.h"
#include "sched/task.h"
#include "types.h"

void task_a(void)
{
  while (1)
  {
    uart_puts("task A running\n");
    for (uint64_t i = 0; i < 500000000; i++)
      ;
  }
}

void task_b(void)
{
  while (1)
  {
    uart_puts("task B running\n");
    for (uint64_t i = 0; i < 500000000; i++)
      ;
  }
}

void kernel_main(void)
{
  uart_init();
  uart_puts("Kernel starting\n");
  timer_init();
  enable_interrupt_controller();
  enable_irq();

  uart_puts("\n--- Initializing PMM ---\n");
  pmm_init();

  uart_puts("\n--- Initializing MMU ---\n");
  mmu_init();
  uart_puts("MMU enabled\n");

  uart_puts("\n--- Initializing Heap ---\n");
  heap_init();
  uart_puts("\n");

  uart_puts("=== TEST 1: Basic Allocation ===\n");
  void *a = kmalloc(64);
  void *b = kmalloc(128);
  void *c = kmalloc(32);
  uart_puts("a (64B)  : ");
  uart_hex((uint64_t)a);
  uart_puts("\n");
  uart_puts("b (128B) : ");
  uart_hex((uint64_t)b);
  uart_puts("\n");
  uart_puts("c (32B)  : ");
  uart_hex((uint64_t)c);
  uart_puts("\n");

  uart_puts("\n=== TEST 2: Sequential Addresses (Splitting) ===\n");
  uart_puts("b should be a + 64 + sizeof(HeapBlock) (32 bytes) = a + 96\n");
  uart_puts("a + 96   : ");
  uart_hex((uint64_t)a + 96);
  uart_puts("\n");
  uart_puts("b actual : ");
  uart_hex((uint64_t)b);
  uart_puts("\n");

  uart_puts("\n=== TEST 3: Free and Reuse ===\n");
  kfree(b);
  void *d = kmalloc(64);
  uart_puts("After freeing b, kmalloc(64) gave: ");
  uart_hex((uint64_t)d);
  uart_puts("\n");
  uart_puts("Should be same address as b: ");
  uart_hex((uint64_t)b);
  uart_puts("\n");

  uart_puts("\n=== TEST 4: Coalescing Proof ===\n");
  kfree(a);
  kfree(d); /* a and d are adjacent free blocks now — should coalesce */
  void *e = kmalloc(150);
  uart_puts("After freeing a and d (adjacent), kmalloc(150) gave: ");
  uart_hex((uint64_t)e);
  uart_puts("\n");
  uart_puts("Should reuse a's address (coalesced): ");
  uart_hex((uint64_t)a);
  uart_puts("\n");

  uart_puts("\n--- Heap tests complete ---\n\n");

  uart_puts("\n--- Initializing Scheduler ---\n");
  task_init();
  task_create(task_a);
  task_create(task_b);
  uart_puts("Tasks created\n");

  task_run_first();
}
