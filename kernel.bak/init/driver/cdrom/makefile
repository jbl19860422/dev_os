C_FILE=$(wildcard *.c)
OBJ_FILE=$(patsubst %.c,%.o,$(C_FILE))
all:${OBJ_FILE}
%.o:%.c
	gcc ${ALL_INC} -I./ -I../vga -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -c $*.c -o $*.o
clean:
	rm *.o
