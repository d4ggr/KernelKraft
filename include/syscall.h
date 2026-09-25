#pragma once
#include "types.h"

#define SYS_WRITE 0
#define SYS_GETPID 1
#define SYS_EXIT 2
#define MAX_SYSCALLS 3

int64_t sys_write(const char *buff, uint64_t len);
int64_t sys_getpid(void);
void sys_exit(int64_t code);
int64_t syscall_handler(uint64_t syscall_num, uint64_t arg0, uint64_t arg1,
                        uint64_t arg2);
