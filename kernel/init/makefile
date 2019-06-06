include ./makefile.base
ALL_INC+=-I driver/vga -Idriver/cdrom
all:kernel.bin
kernel_init.o:kernel_init.c
	gcc $(ALL_INC) -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -c kernel_init.c -o kernel_init.o
driver/vga/vga.o:driver/vga/vga.c
	make -C driver/vga
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
./init.o:init.c
	gcc $(ALL_INC) -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -c $*.c -o $*.o
kernel.bin:kernel_init.o ./driver/vga/vga.o ./driver/cdrom/cdrom16.o ./driver/cdrom/cdrom32.o ./memory/memory32.o driver/vga/vga16.o ./ata/ata.o ./driver/pci/pci.o driver/8259/8259.o interrupt/interrupt.o interrupt/interrupt_entry.o init.o
	ld -T kernel.ld kernel_init.o driver/vga/vga.o ./memory/memory32.o driver/vga/vga16.o  interrupt/interrupt_entry.o driver/8259/8259.o interrupt/interrupt.o init.o ${ALL_LIB} ${ALL_LINK} -o kernel
	objcopy	-S -O binary kernel kernel.bin
clean:
	rm *.o
	rm kernel.bin
