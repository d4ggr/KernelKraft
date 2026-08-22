.section ".text.boot"
.global _start
_start:
mrs x0, mpidr_el1
and x0, x0, #0xFF
cbz x0, master
proc_hang:
    wfe
    b proc_hang

master:
    mov x0, #0x3c5
    msr spsr_el2, x0
    adr x0, el1_entry
    msr elr_el2, x0
    eret

el1_entry:
    adr x0, vectors
    msr vbar_el1,x0
    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0
    cbz x1, skip_bss
    

bss_loop:
    str xzr, [x0], #8
    subs x1, x1, #8
    b.gt bss_loop
skip_bss:

ldr x0, =_stack_top
mov sp,x0

bl kernel_main
b proc_hang

