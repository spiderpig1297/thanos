NASM = /usr/bin/nasm
QEMU = qemu-system-i386
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
CFLAGS = -g

C_SOURCE = $(wildcard kernel/*.c drivers/*.c)
C_HEADERS = $(wildcard kernel/*.h drivers/*.h)
C_OBJ = ${C_SOURCE:.c=.o}

all: run

os-image.bin: boot/boot_sect_main.bin kernel.bin 
	cat $^ > $@

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${C_OBJ}
	$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${C_OBJ}
	$(LD) -o $@ -Ttext 0x1000 $^

run: os-image.bin
	$(QEMU) -fda $<

debug: os-image.bin kernel.elf
	$(QEMU) -s -fda $< &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o : %.c ${C_HEADERS}
	$(CC) -ffreestanding -c $< -o $@

%.o : %.asm
	$(NASM) $< -f elf -o $@

%.bin : %.asm
	$(NASM) $< -f bin -I '../../16bit/' -o $@
	
kernel_entry.o: kernel_entry.asm
	$(NASM) $< -f elf -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o