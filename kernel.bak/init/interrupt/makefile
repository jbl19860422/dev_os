C_FILE=$(wildcard *.c)
C_OBJ_FILE=$(patsubst %.c,%.o,$(C_FILE))
A_OBJ_FILE=$(patsubst %.s,%.o,$(wildcard *.s))
all:$(C_OBJ_FILE) $(A_OBJ_FILE)
.c.o:%.c
	gcc ${ALL_INC} -I./ -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -c $< -o $*.o
.s.o:%.s
	nasm -f elf $< -o $*.o
clean:
	rm *.o
