#pragma once

#include "types.h"

#define TASK_RUNNING 0
#define TASK_READY 1
#define TASK_ZOMBIE 2

#define MAX_TASKS 8
#define TASK_STACK_SIZE 4096

struct Task
{
  uint64_t sp;
  uint64_t x30;
  uint64_t state;
  uint64_t *stack;
  uint64_t entry;
};

void task_init(void);
struct Task *task_create(void (*entry)(void));
void schedule(void);
void task_run_first(void);
