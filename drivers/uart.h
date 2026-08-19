#pragma once 
void uart_init(void);
void uart_putc(char c);
void uart_puts(char* str);
void uart_hex(uint64_t value);
