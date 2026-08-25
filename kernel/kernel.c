#include "types.h"
#include "drivers/uart.h"
#include "drivers/timer.h"
#include "irq.h"

void kernel_main(void){
    uart_init();
    uart_puts("Kernel starting\n");
    timer_init();
    uart_puts("Timer initialized\n");
    enable_interrupt_controller();    

    enable_irq();

    while(1){
        for(uint64_t i = 0 ; i < 100000000 ; i++){

        }
        uart_puts("kernel: foreground loop running\n");
    }

}
