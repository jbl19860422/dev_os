include ./makefile.base
ALL_INC+=-I./ -I./driver/vga -I./ata -I./driver/pci -I./driver/cdrom -I./base/inc -I./memory -m32 -march=i386
all:loader.bin
main.o:main.c
	gcc $(ALL_INC) -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -c main.c -o main.o
main1.5.o:main1.5.c
	gcc $(ALL_INC) -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -c main1.5.c -o main1.5.o
loader.o:loader.s
	nasm -felf loader.s -o loader.o
driver/vga/vga.o:driver/vga/vga.c
	make -C driver/vga
switch_mode.o:switch_mode.s
	nasm -felf switch_mode.s -o switch_mode.o
./memory/memory32.o:memory/memory32.c
	make -C memory
./ata/ata.o:ata/ata.c
	make -C ata
./driver/cdrom/cdrom16.o:driver/cdrom/cdrom16.c driver/cdrom/cdrom16.c
	make -C driver/cdrom
./driver/cdrom/cdrom32.o:driver/cdrom/cdrom16.c driver/cdrom/cdrom32.c
	make -C driver/cdrom
./driver/pci/pci.o:driver/pci/pci.c
	make -C driver/pci
./driver/8259/8259.o:driver/8259/8259.c
	make -C driver/8259
./interrupt/interrupt.o:interrupt/interrupt.c
	make -C interrupt
./driver/mouse/mouse.o:driver/mouse/mouse.c
	make -C driver/mouse
./init.o:init.c
	gcc $(ALL_INC) -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -c $*.c -o $*.o
loader.bin:loader.o main1.5.o ./driver/vga/vga.o ./driver/cdrom/cdrom16.o ./driver/cdrom/cdrom32.o switch_mode.o  ./memory/memory32.o driver/vga/vga16.o ./ata/ata.o ./driver/pci/pci.o driver/8259/8259.o interrupt/interrupt.o interrupt/interrupt_entry.o init.o ata/ata.o ./driver/mouse/mouse.o
	ld -melf_i386 -T loader.ld loader.o switch_mode.o main1.5.o driver/vga/vga.o  ./memory/memory32.o ata/ata.o driver/vga/vga16.o  interrupt/interrupt_entry.o ./driver/cdrom/cdrom16.o ./driver/cdrom/cdrom32.o ./driver/pci/pci.o driver/8259/8259.o interrupt/interrupt.o init.o ./driver/mouse/mouse.o ${ALL_LIB} ${ALL_LINK} -o loader.bin
#	objcopy	-S -O binary loader loader.bin
clean:
	rm *.o
	rm loader.bin
