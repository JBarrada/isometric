CFLAGS=-std=gnu99 -ffreestanding -fbuiltin -Wall -Wextra -g -fdiagnostics-color
LIBS=-Iinclude -nostdlib

AS=/home/swdev/cross-compile/i686-elf-as
CC=/home/swdev/cross-compile/usr/bin/i686-elf-gcc

PROJDIR=machine core_c core_else game

SRCFILES:= $(shell find $(PROJDIRS) -type f -name "*.c")
OBJFILES:= $(patsubst %.c,%.o,$(SRCFILES)) core_else/interrupt_handler.o core_else/boot.o machine/memcpy.o machine/memset.o

all: clean advent.iso
	
%.o: %.c
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@
	
%.o: %.s
	$(AS) -c $< -o $@

%.o: %.S
	$(CC) -c $< -o $@
	
advent.bin: $(OBJFILES)
	$(LD) -T core_else/linker.ld -o $@ $(OBJFILES) $(LIBS)
	
	
advent.iso: core_else/grub.cfg advent.bin
	mkdir -p isodir
	mkdir -p isodir/boot
	cp advent.bin isodir/boot/advent.bin
	mkdir -p isodir/boot/grub
	cp core_else/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir
	rm -R isodir
	
clean:
	rm -f *.o *.iso advent.bin $(OBJFILES)
