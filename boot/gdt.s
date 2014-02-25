; gdt.s
; Wolfrath/Kriewall, 2013
;
; Global descriptor table and interrupt descriptor table

[GLOBAL gdt_flush]

gdt_flush:
    mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lgdt [eax]        ; Load the new GDT pointer
    jmp 0x08:.flush   ; 0x08 is the offset to code segment

.flush:
	mov ax, 0x10      ; 0x10 is the offset in the GDT data segment
	mov ds, ax        ; Load all data segment selectors
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret

[GLOBAL idt_flush]

idt_flush:
    mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter.
    lidt [eax]        ; Load the IDT pointer.
    ret

[GLOBAL tss_flush]

tss_flush:
   mov ax, 0x2B      ; Load the index of our TSS structure - The index is
                     ; 0x28, as it is the 5th selector and each is 8 bytes
                     ; long, but we set the bottom two bits (making 0x2B)
                     ; so that it has an RPL of 3, not zero.
   ltr ax            ; Load 0x2B into the task state register.
   ret
