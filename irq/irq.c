#include "drivers/uart.h"
#include "drivers/timer.h"
#include "irq.h"
#include "peripherals/irq.h"
#include "syscall.h"
#include "types.h"

const char* entry_error_messages[] = {
    "SYNC_INVALID_EL1t",
    "IRQ_INVALID_EL1t",
    "FIQ_INVALID_EL1t",
    "ERROR_INVALID_EL1t",

    "SYNC_INVALID_EL1h",
    "IRQ_INVALID_EL1h",
    "FIQ_INVALID_EL1h",
    "ERROR_INVALID_EL1h",

    "SYNC_INVALID_EL0_64",
    "IRQ_INVALID_EL0_64",
    "FIQ_INVALID_EL0_64",
    "ERROR_INVALID_EL0_64",

    "SYNC_INVALID_EL0_32",
    "IRQ_INVALID_EL0_32",
    "FIQ_INVALID_EL0_32",
    "ERROR_INVALID_EL0_32"
};

void show_invalid_entry_message(int type, uint64_t esr, uint64_t addr){
      uart_puts("EXEPTION: ");
      uart_puts((char*)entry_error_messages[type]);
      uart_puts("\nAddress: ");
      uart_hex(addr);
      uart_puts("\n");
}

void enable_interrupt_controller(void){
    ENABLE_IRQS_1 = SYSTEM_TIMER_IRQ_1 ;
}

void handle_irq(void){
  if(IRQ_PENDING_1 == SYSTEM_TIMER_IRQ_1){
    handle_timer_irq();
    return ;
  }
}

void handle_sync_exception(struct Trapframe *regs)
{
  uint64_t esr;
  asm volatile("mrs %0, esr_el1" : "=r"(esr));
  uint64_t ec = (esr >> 26) & 0x3F;

  if (ec == 0x15)
  {
    regs->x[0] = syscall_handler(regs->x[8], regs->x[0], regs->x[1], regs->x[2]);
  }
  else
  {
    uart_puts("UNKNOWN SYNC EXCEPTION\n");
    show_invalid_entry_message(4, esr, regs->elr_el1);
    while (1);
  }
}
