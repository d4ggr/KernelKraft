#include "mm/pmm.h"
#include "types.h"
#include "drivers/uart.h"

extern uint8_t _end;

#define MEM_END   0x3F000000
#define PAGE_SIZE       4096  //size of each page
#define SIZE_ARRAY      4096  //for array to be of uint64_t
                                   
static uint64_t pmm_bitmap[SIZE_ARRAY];
static uint64_t mem_start;
static uint64_t total_pages;
static uint64_t free_pages;

void pmm_init(void){
    mem_start = ((uint64_t)&_end + 4095) & (~4095) ;
    total_pages = (MEM_END - mem_start)/PAGE_SIZE ;
    free_pages = total_pages;
    for(uint64_t i=0; i<SIZE_ARRAY; i++){
        pmm_bitmap[i] = 0;
    }
    uart_puts("mem_start : ");
    uart_hex(mem_start);
    uart_puts("\nmem_end : ");
    uart_hex(MEM_END);
}

void* pmm_alloc(void){
    for(uint64_t i = 0; i<SIZE_ARRAY; i++){
        if(pmm_bitmap[i] != ~0ULL){
            for(uint64_t b = 0; b< 64; b++){
                if((pmm_bitmap[i] & (1ULL << b)) == 0){
                    pmm_bitmap[i] |= (1ULL <<b) ;
                    free_pages-- ;
                    void* alloc_mem_addr = (void*)(mem_start +(((i<<6)+b)*PAGE_SIZE));

                    uint64_t *p = (uint64_t*)alloc_mem_addr;
                    for(uint64_t k = 0; k < (PAGE_SIZE / 8); k++){
                        p[k] = 0;
                    }

                    return alloc_mem_addr;
                }
            }
        }
    }
    return NULL;
}

void pmm_free(void* ptr){
    uint64_t addr = (uint64_t)ptr;
    if(addr < mem_start || addr >= MEM_END || (addr & (PAGE_SIZE - 1)) != 0){
        return;
    }

    uint64_t page_idx = (addr - mem_start) / PAGE_SIZE;
    uint64_t arr_idx = page_idx >> 6;
    uint64_t bit_idx = page_idx & 63;

    if(pmm_bitmap[arr_idx] & (1ULL << bit_idx)){
        pmm_bitmap[arr_idx] &= ~(1ULL << bit_idx);
        free_pages++;
    }
}

uint64_t pmm_get_free_page_count(void){
    return free_pages;
}
