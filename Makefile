#
# Joel Wolfrath, 2014
# Makefile for the kernel
#

CC= gcc

SOURCES= 	bootstrap.o \
			boot/acpi.o \
			boot/interrupt.o \
			boot/isr.o \
			boot/gdt.o \
			boot/init_tables.o \
			core/console.o \
			core/io.o \
			core/string.o \
			mm/mmap.o \
			mm/paging.o \
			mm/alloc.o \
			mm/kheap.o \
			mm/ppa.o \
			lib/list.o \
			util/debug.o \
			drivers/kbd.o \
			proc/pic.o \
			proc/proc.o \
			proc/scheduler.o \
			proc/stack.o \
			main.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector

LDFLAGS=-Tlink.ld

ASFLAGS=-felf

all: $(SOURCES) link

clean:
	find . -type f -name "*.o" -print0 | xargs -0 rm -rf
	rm kernel

# -M to linker for printout

link:
	ld -M $(LDFLAGS) -o kernel $(SOURCES) > linker.out

.S.o:
	$(CC) -c $<

.s.o:
	nasm $(ASFLAGS) $<

