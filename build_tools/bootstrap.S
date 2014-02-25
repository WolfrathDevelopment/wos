; wOS - Joel Wolfrath, 2012
; boot.s -- Kernel start location; defines multiboot header,
; uses GRUB as the bootloader

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel on a page boundary
MBOOT_MEM_INFO      equ 1<<1
MBOOT_AOUT_KLUDGE equ 1<<16 ; gimme the multiboot header
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]                       ; 32 bit instructions

[GLOBAL mboot] 
[GLOBAL start]
[EXTERN code]
[EXTERN bss]
[EXTERN end]
[EXTERN main]
[EXTERN init_pgdir]		; initial page directory - maps high half

mboot:
  dd  MBOOT_HEADER_MAGIC        ; GRUB will search for this value on each
                                ; 4-byte boundary in the kernel file
  dd  MBOOT_HEADER_FLAGS        ; How GRUB should load file settings
  dd  MBOOT_CHECKSUM
   
  dd  mboot
  dd  code
  dd  bss
  dd  end
  dd start              ; Kernel entry point (initial EIP).
  start = init_pg + 0xC0000000

k_init:
	push ebx
	cli
	call main
	jmp $

init_pg:
	mov cr4, eax
	or eax, 0x00000010
	mov eax, cr4		; set page size extension

	mov [init_pgdir], eax
	mov eax, cr3		; point to initial page directory

	mov cr0, eax
	or eax, 0x80000000
	mov eax, cr0		; begin paging

	lea ebx, [k_init]
	jmp ebx 			; jump to kernel instruction in high half
