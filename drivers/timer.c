#include "drivers/timer.h"
#include "drivers/uart.h"
#include "peripherals/timer.h"

void timer_init(void){
    TIMER_C1 = TIMER_CLO + 200000;
}

void handle_timer_irq(void){
    TIMER_C1 = TIMER_CLO + 200000;

    TIMER_CS = (1<<1);

    uart_puts("TIMER INTERRUPT RECEIVED\n");
}
