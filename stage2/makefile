kernel.bin:head.s
	as --32 -march=i386 head.s -o head.o
	ld -melf_i386 -T kernel.ld head.o -o kernel
	objcopy -S -O binary kernel  kernel.bin
