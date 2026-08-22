#include "types.h"
#include "uart.h"

void kernel_main(void){
    uart_init();
    uart_puts("Kernel starting\n");
    
    asm volatile("svc #0");

    uart_puts("never reached.\n");


}
