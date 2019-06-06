all:vga.o vga16.o
%.o:%.c
	gcc ${ALL_INC} -I./ -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -c $*.c -o $*.o
clean:
	rm *.o
