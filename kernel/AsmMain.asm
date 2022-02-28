extern kernel_main_stack
extern KernelMain_NewStack

;ゼロからのOS自作入門p187より
global KernelMain
KernelMain:
    mov rsp, kernel_main_stack + 1024 * 1024 ;スタックポインタの移動。
    call KernelMain_NewStack

.fin:
    hlt
    jmp .fin