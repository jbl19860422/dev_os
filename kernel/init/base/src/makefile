include ../../makefile.base
C_FILE=$(wildcard *.c)
OBJ_FILE=$(patsubst %.c,%.o,$(C_FILE))
all:libbase.a
libbase.a:${OBJ_FILE}
	ar rv libbase.a ${OBJ_FILE}
	cp libbase.a ../lib 
%.o:%.c
	gcc ${ALL_INC} -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -c $< -o $@
clean:
	rm *.o
	rm libbase.a
