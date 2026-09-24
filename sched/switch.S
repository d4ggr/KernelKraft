.global context_switch
context_switch:
    mov  x9, sp
    str  x9,  [x0, #0]
    str  x30, [x0, #8]

    ldr  x9,  [x1, #0]
    mov  sp,  x9
    ldr  x30, [x1, #8]

    ret

.global task_run_first_asm
task_run_first_asm:
    ldr  x9,  [x0, #0]
    mov  sp,  x9
    ldr  x30, [x0, #8]
    ret
