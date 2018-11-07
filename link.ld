/*
 * link.ld
 * Joel Wolfrath, 2013
 *
 * Linker file for kernel:
 * Maps the kernel as a higher half kernel
 */

OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

SECTIONS
{
    /* The kernel will live at 3GB + 1MB in the virtual address space, */
    /* which will be mapped to 1MB in the physical address space. */
    /* Note that we page-align the sections. */
    . = 0xC0100000;
    PROVIDE(kern_start = . - 0xC0000000);
    /* Add a symbol that indicates the start address of the kernel. */
    _kernel_start = .;
    .text ALIGN (4K) : AT (ADDR (.text) - 0xC0000000)
    {
        *(.multiboot)
        *(.text)
    }
    .rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000)
    {
        *(.rodata)
    }
    .data ALIGN (4K) : AT (ADDR (.data) - 0xC0000000)
    {
        *(.data)
    }
    .bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000)
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }
    /* Add a symbol that indicates the end address of the kernel. */
    _kernel_end = .;
    PROVIDE(kern_end = . - 0xC0000000);
}
