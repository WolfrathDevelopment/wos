# Makefile for the kernel

SOURCES= bootstrap.o boot/interrupt.o boot/isr.o boot/gdt.o  boot/init_tables.o  core/console.o core/io.o core/string.o core/cpu.o core/spinlock.o mm/mmap.o mm/paging.o util/debug.o core/atomic.o drivers/kbd.o main.o 
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm *.o kernel

link:
	ld -M $(LDFLAGS) -o kernel $(SOURCES)
.S.o:
	gcc -c $<
.s.o:
	nasm $(ASFLAGS) $<
