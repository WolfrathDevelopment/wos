#
# bootstrap.S
# Wolfrath/Kriewall, 2013
#
# Kernel start location
# Uses GRUB as the bootloader
# Sets up paging and starts kernel executing at 0xC0000000
# 

.globl mboot 
.globl start
.extern main					# C entrypoint
.extern init_pgdir				# initial page directory

.set ALIGN,    1 << 0				# Tell GRUB to align modules
.set MEMINFO,  1 << 1			# Get mmap from GRUB
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS) 

.set KERN_VIR_BASE, 0xC0000000	# Address to map kernel to 

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
	push %ecx				# Pass Multiboot Info to main
	cli
	call main
	hlt

init_pg:
	movl %ebx, %ecx

	movl %cr4, %ebx
	orl $0x00000010, %ebx
	movl %ebx, %cr4			# Set 4MB pages

	movl $(init_pgdir - KERN_VIR_BASE), %ebx
	movl %ebx, %cr3			# Give cr3 address of page directory

	movl %cr0, %ebx
	orl $0x80000000, %ebx
	movl %ebx, %cr0			# Begin paging

	leal k_init, %ebx
	jmp *%ebx				# Absolute jump!
