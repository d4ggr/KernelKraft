#include "syscall.h"
#include "drivers/uart.h"
#include "sched/task.h"

extern uint64_t curr_task_idx;
extern struct Task p_table[MAX_TASKS];

int64_t sys_write(const char *buff, uint64_t len)
{
  for (uint64_t i = 0; i < len; i++)
  {
    uart_putc(buff[i]);
  }
  return len;
}

int64_t sys_getpid(void)
{
  return curr_task_idx;
}

void sys_exit(int64_t code)
{
  p_table[curr_task_idx].state = TASK_ZOMBIE;
  schedule();
  while (1)
    ;
}

static void *syscall_table[MAX_SYSCALLS] = {
    [SYS_WRITE] = sys_write,
    [SYS_GETPID] = sys_getpid,
    [SYS_EXIT] = sys_exit,
};

int64_t syscall_handler(uint64_t syscall_num, uint64_t arg0, uint64_t arg1,
                        uint64_t arg2)
{
  if (syscall_num >= MAX_SYSCALLS)
  {
    return -1;
  }
  void *fn = syscall_table[syscall_num];
  if (!fn)
  {
    return -1;
  }
  int64_t (*handler)(uint64_t, uint64_t, uint64_t) =
      (int64_t(*)(uint64_t, uint64_t, uint64_t))fn;

  return handler(arg0, arg1, arg2);
}
