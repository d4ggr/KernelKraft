#include "sched/task.h"
#include "mm/pmm.h"
#include "types.h"

struct Task p_table[MAX_TASKS];
uint64_t n_active_tasks;
uint64_t curr_task_idx;

static void task_trampoline(void)
{
  asm volatile("msr daifclr, #2");
  void (*fn)(void) = (void (*)(void))p_table[curr_task_idx].entry;
  fn();
}

void task_init(void)
{
  for (uint64_t i = 0; i < MAX_TASKS; i++)
  {
    p_table[i].sp    = 0;
    p_table[i].x30   = 0;
    p_table[i].state = TASK_ZOMBIE;
    p_table[i].stack = NULL;
    p_table[i].entry = 0;
  }
}

struct Task *task_create(void (*entry)(void))
{
  for (uint64_t i = 0; i < MAX_TASKS; i++)
  {
    if (p_table[i].state == TASK_ZOMBIE)
    {
      p_table[i].stack = pmm_alloc();
      p_table[i].sp    = (uint64_t)p_table[i].stack + TASK_STACK_SIZE;
      p_table[i].x30   = (uint64_t)task_trampoline;
      p_table[i].entry = (uint64_t)entry;
      p_table[i].state = TASK_READY;
      n_active_tasks++;
      return &p_table[i];
    }
  }
  return NULL;
}

extern void context_switch(struct Task *prev, struct Task *next);

void schedule(void)
{
  uint64_t next = curr_task_idx;
  for (uint64_t i = 0; i < n_active_tasks; i++)
  {
    next = (next + 1) % n_active_tasks;
    if (p_table[next].state != TASK_ZOMBIE)
      break;
  }

  if (next == curr_task_idx)
    return;

  uint64_t prev = curr_task_idx;
  curr_task_idx = next;

  context_switch(&p_table[prev], &p_table[next]);
}

extern void task_run_first_asm(struct Task *t);

void task_run_first(void)
{
  curr_task_idx = 0;
  task_run_first_asm(&p_table[0]);
}
