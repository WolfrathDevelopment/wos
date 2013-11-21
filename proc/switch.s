[GLOBAL] perform_context_switch
[EXTERN] current_page_directory

perform_context_switch:

    ; switch pgdir

	;mov cr3, current_page_directory

	; switch stack



    ; reload the original data segment descriptor

    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa

    ; Cleans up the pushed error code and pushed by the timer

    add esp, 8
    sti

    ; pop CS, EIP, EFLAGS, SS, and ESP

    iret


