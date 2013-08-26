; wOS - Joel Wolfrath, 2012
; boot.s -- Kernel start location; defines multiboot header,
; uses GRUB as the bootloader

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel on a page boundary
MBOOT_MEM_INFO      equ 1<<1
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]                       ; 32 bit instructions

[GLOBAL mboot]                  ; C code can now access mboot
[EXTERN code]
[EXTERN bss]
[EXTERN end]

mboot:
  dd  MBOOT_HEADER_MAGIC        ; GRUB will search for this value on each
                                ; 4-byte boundary in the kernel file
  dd  MBOOT_HEADER_FLAGS        ; How GRUB should load file settings
  dd  MBOOT_CHECKSUM
   
  dd  mboot
  dd  code
  dd  bss
  dd  end
  dd  start                     ; Kernel entry point (initial EIP).

[GLOBAL start]                  ; Kernel entry point.
[EXTERN main]                   ; C code entry point.

start:
  push    esp
  push    ebx                   ; Load multiboot header location
  cli                           ; Disable interrupts.
  call main
  jmp $
