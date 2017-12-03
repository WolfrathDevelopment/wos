#
# bootstrap.S
# Joel Wolfrath, 2013
#
# Kernel start location
# Uses GRUB as the bootloader; GRUB calls into this file
# Sets up paging and starts kernel executing at 0xC0000000
#

.globl mboot
.globl start
.extern main					# C entrypoint
.extern init_pgdir				# initial page directory
.extern init_pgtbl				# initial page table
.extern init_stack				# initial stack page

.set ALIGN,    1 << 0				# Tell GRUB to align modules
.set MEMINFO,  1 << 1				# Get mmap from GRUB
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.set KERN_VIR_BASE, 0xC0000000			# Address to map kernel to

#define STACK_SIZE			0x2000

mboot:
	.long  MAGIC
	.long  FLAGS
	.long  CHECKSUM

	.globl  mboot
	.globl  code
	.globl  bss
	.globl  end
	.globl start

	start = (init_pg - KERN_VIR_BASE)	# PHYSICAL address to jump to

k_init:
#	movl $(stack + STACK_SIZE), %esp
#	movl $(stack + STACK_SIZE), %ebp
	movl $(init_stack + STACK_SIZE), %esp
	movl $(init_stack + STACK_SIZE), %ebp
	push %ecx				# Pass Multiboot Info to main
	push %ebp				# Pass stack base
#	push %esp				# Pass stack pointer
	cli
	call main
	hlt

init_pg:

	movl %ebx, %ecx				# Save multiboot info in ecx

	movl $(init_pgtbl - KERN_VIR_BASE), %ebx
	xorl %eax, %eax

	.rept 1024
		orl $3, %eax			# set permissions
		movl %eax, (%ebx)		# add entry to pgtbl
		subl $3, %eax
		addl $0x1000, %eax		# set next page
		addl $4, %ebx			# set next tbl index
	.endr					# populate page table

	movl $(init_pgtbl - KERN_VIR_BASE), %ebx
	movl $(init_pgdir - KERN_VIR_BASE), %eax
	orl $3, %ebx
	movl %ebx, (%eax)
	addl $0xC00, %eax
	movl %ebx, (%eax)

#	movl %cr4, %ebx
#	orl $0x00000010, %ebx
#	movl %ebx, %cr4				# Set 4MiB pages

	movl $(init_pgdir - KERN_VIR_BASE), %ebx
	movl %ebx, %cr3				# Give cr3 address of page directory

	movl %cr0, %ebx
	orl $0x80000000, %ebx
	movl %ebx, %cr0				# Begin paging

	leal k_init, %ebx
	jmp *%ebx				# Absolute jump!

#.comm   stack, STACK_SIZE
