#pragma once
#include "types.h"

struct Trapframe
{
  uint64_t x[31];
  uint64_t padding;
  uint64_t elr_el1;
  uint64_t spsr_el1;
};

void show_invalid_entry_message(int type, uint64_t esr, uint64_t addr);
void handle_irq(void);
void enable_interrupt_controller(void);
void enable_irq(void);
void disable_irq(void);
void handle_sync_exception(struct Trapframe *regs0);
