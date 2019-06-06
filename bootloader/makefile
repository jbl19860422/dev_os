INC+=./include
all:loader.bin head.bin
main.o:main.c
	gcc -I $(INC) -march=i386 -m32 -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -c main.c -o main.o
loader.o:loader.s
	nasm -felf loader.s -o loader.o
head.bin:head.S
	as head.S -o head.o
	ld -T head.ld head.o -o head
	objcopy -S -O binary head head.bin	
string.o:string.s
	nasm -felf string.s -o string.o
loader.bin:string.o main.o loader.o
	ld -melf_i386 -T loader.ld loader.o string.o main.o -o loader
	objcopy	-S -O binary loader loader.bin
clean:
	rm *.o
	rm head.bin
	rm loader.bin
