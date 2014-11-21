#
# Joel Wolfrath, 2014
# Makefile for the kernel
#

ifeq ($(MAKE),)
MAKE= make
endif

ifeq ($(RM),)
RM= rm
endif

CC= gcc
CFLAGS= -c -O2 \
		-nostdlib -nostdinc -fno-builtin -fno-stack-protector

LDFLAGS= -Tlink.ld

NASM= nasm
ASMFLAGS= -felf

SOURCES= $(shell find * -type f -name '*.[cSs]')
OBJECTS= $(patsubst %.c,%.o, \
			$(patsubst %.s,%.o, \
				$(patsubst %.S, %.o,$(SOURCES))))

OUTPUT= kernel
OUTFILE= build_dump

.PHONY: all clean

all: $(SOURCES) $(OUTPUT)
	@echo "Done."

# -M to linker for printout

$(OUTPUT): $(OBJECTS)
	@echo "Linking..."
	@ld -M $(LDFLAGS) -o $@ $(OBJECTS) >> $(OUTFILE) 2>&1
	@- $(RM) $(OBJECTS)

.c.o:
	@echo "CC $<"
	@$(CC) $(CFLAGS) $< -o $@ >> $(OUTFILE) 2>&1

.S.o:
	@echo "CC $<"
	@$(CC) -c $< >> $(OUTFILE) 2>&1

.s.o:
	@echo "NASM $<"
	@$(NASM) $(ASMFLAGS) $< >> $(OUTFILE) 2>&1

clean:
	@- $(RM) $(OBJECTS)
	@- $(RM) $(OUTPUT)
	@- $(RM) $(OUTFILE)
