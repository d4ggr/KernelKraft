#include "types.h"
#include "drivers/uart.h"
#include "drivers/timer.h"
#include "irq.h"
#include "mm/pmm.h"

void kernel_main(void){
    uart_init();
    uart_puts("Kernel starting\n");
    timer_init();
    uart_puts("Timer initialized\n");
    enable_interrupt_controller();    

    enable_irq();

    uart_puts("\n--- Initializing PMM ---\n");
    pmm_init();

    uart_puts("\nInitial Free Pages: ");
    uart_hex(pmm_get_free_page_count());
    uart_puts("\n");

    void *p1 = pmm_alloc();
    void *p2 = pmm_alloc();
    void *p3 = pmm_alloc();

    uart_puts("Allocated Page 1: ");
    uart_hex((uint64_t)p1);
    uart_puts("\nAllocated Page 2: ");
    uart_hex((uint64_t)p2);
    uart_puts("\nAllocated Page 3: ");
    uart_hex((uint64_t)p3);
    uart_puts("\n");

    uart_puts("Freeing Page 2...\n");
    pmm_free(p2);

    void *p4 = pmm_alloc();
    uart_puts("Allocated Page 4 (should reuse Page 2): ");
    uart_hex((uint64_t)p4);
    uart_puts("\n\n");

    while(1){
        for(uint64_t i = 0 ; i < 200000000 ; i++){
        }
        uart_puts("kernel: foreground loop running\n");
    }

}
