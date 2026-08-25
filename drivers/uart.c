#include "peripherals/uart.h"
#include "types.h"
#include "drivers/uart.h"

void uart_init(void){
    UART_CR = 0;
    UART_IBRD = 26;
    UART_FBRD = 2;
    UART_LCRH = (1<<4) | (3<<5);
    UART_CR = (1<<0) | (1<<8) | (1<<9);
    return ;
}

void uart_putc(char c){
    while((UART_FR & (1<< 5))){};
        UART_DR = c ;
}
void uart_puts(char* str){
  
      while(*str !='\0'){
        uart_putc(*str);
        str++;
      }
}

void uart_hex(uint64_t value){
    for(uint32_t i = 0 ; i< 16 ; i++){
        uint8_t each_byte = (value >> 4*(15-i)) & 0x0F;
        if(each_byte > 9){
          uart_putc((char)(each_byte) + 55);

        }
        else{
          uart_putc((char)(each_byte) + 48);
        }
    }
}
