include ../makefile.base
C_FILE=$(wildcard *.c)
NASM_FILE=$(wildcard *.s)
OBJ_FILE_C=$(patsubst %.c,%.o,$(C_FILE))
OBJ_FILE_S=$(patsubst %.s,%.o,$(NASM_FILE))
all:$(OBJ_FILE_C) $(OBJ_FILE_S)
.s.o:
	nasm -f elf $*.s -o $@
%.o:%.c
	echo ${NASM_FILE}
	gcc ${ALL_INC} -I./ -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -c $*.c -o $*.o
clean:

	rm *.o
