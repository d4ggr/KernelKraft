#pragma once 
#include "types.h"

void show_invalid_entry_message(int type,uint64_t esr, uint64_t addr);
void handle_irq(void);
void enable_interrupt_controller(void);
void enable_irq(void);
void disable_irq(void);
