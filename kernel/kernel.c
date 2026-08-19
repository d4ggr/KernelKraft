#include "types.h"
#include "uart.h"

void kernel_main(void){
    uart_init();
    uart_puts("Hello World\n");
    uart_hex(0xFFFF0000FFFF1234);
}
