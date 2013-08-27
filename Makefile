# Makefile for the kernel

SOURCES=bootstrap.o core/console.o core/io.o core/descriptor_tables.o core/interrupt.o core/string.o core/isr.o core/gdt.o main.o 
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	-rm *.o kernel

link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
