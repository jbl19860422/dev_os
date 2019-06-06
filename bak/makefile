INC+=./include
all:loader.bin
main.o:main.c
	gcc -I $(INC) -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector -c main.c -o main.o
loader.o:loader.s
	nasm -felf loader.s -o loader.o
string.o:string.s
	nasm -felf string.s -o string.o
loader.bin:string.o main.o loader.o
	ld -T loader.ld -static loader.o string.o main.o -o loader
	objcopy	-S -O binary loader loader.bin
clean:
	rm *.o
	rm loader.bin
