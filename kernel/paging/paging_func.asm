global SetCR3 ;void SetCR3(uint64_t value);
SetCR3:
    mov cr3,rdi
    ret