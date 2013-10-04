/* Link.ld -- Linker script for the kernel */

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)

ENTRY(start)
SECTIONS
{
	PROVIDE(kern_start = 0x100000);
	. = 0xC0100000;

	.text : AT(ADDR(.text) - 0xC0000000) {
		code = .;
		*(.text)
		*(.rodata*)
	}

	.data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000) {
		*(.data)
	}

	.bss : AT(ADDR(.bss) - 0xC0000000) {
		bss = .;
		*(.bss)
	}

	end = .;
	PROVIDE(kern_end = .);
}
