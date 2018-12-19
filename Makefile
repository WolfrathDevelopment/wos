#
# Joel Wolfrath, 2014
# Makefile for the kernel
#

CC= gcc

SOURCES=bootstrap.o \
        arch/spinlock.o \
        boot/interrupt.o \
        boot/isr.o \
        boot/gdt.o \
        boot/init_tables.o \
        lib/console.o \
        lib/io.o \
        lib/string.o \
        mm/mmap.o \
        mm/paging.o \
        mm/kheap.o \
        mm/alloc.o \
        mm/ppa.o \
        lib/list.o \
        tools/debug.o \
        drivers/kbd.o \
        proc/pic.o \
        proc/proc.o \
        proc/syscall.o \
        proc/scheduler.o \
        proc/stack.o \
        proc/thread.o \
        io/bus.o \
        io/mmio.o \
        main.o

CFLAGS= -I include/ -nostdlib -nostdinc -fno-pic -fno-builtin -fno-stack-protector -m32

LDFLAGS=-m elf_i386 -Tlink.ld

ASFLAGS=-felf32

all: $(SOURCES) link

clean:
	find . -type f -name "*.o" -print0 | xargs -0 rm -rf
	rm kernel 

# -M to linker for printout

link:
	ld -M $(LDFLAGS) -o kernel $(SOURCES) > linker.out

.S.o:
	$(CC) -c -m32 $<

.s.o:
	nasm $(ASFLAGS) $<

