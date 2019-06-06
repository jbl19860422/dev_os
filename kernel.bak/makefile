SUBDIRS	:=	init
SUBOBJS := init/init.o
TARGET	:= 	kernel.img
.PHONY:all $(SUBDIRS) clean	#Î±Ä¿±ê
AOBJS	:=	$(patsubst %.s,%.o,$(wildcard *.s))
COBJS	:=	$(patsubst %.c,%.o,$(wildcard *.c))
OBJS	:=	$(AOBJS) $(COBJS)

CC	:=	gcc
ASM	:=	nasm
AR	:=	ar
LD	:=	ld

CC_FLAGS := -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -c
ASM_FLAGS := -f elf
AR_FLAGS := rv
LD_FLAGS := -T kernel.ld

all: $(TARGET)
$(TARGET):$(SUBDIRS) $(OBJS)
	@for dir in $(SUBDIRS);\
		do $(MAKE) -C $$dir all CC=$(CC) AR=$(AR) TOPDIR=$(PWD); \
	done
	$(LD) $(LD_FLAGS) $(OBJS) $(SUBOBJS) -o $@.o;
	objcopy	-S -O binary $(TARGET).o $@;
.s.o:%.s
	$(ASM) $(ASM_FLAGS) $< -o $@
.c.o:%.c
	$(CC) $(C_FLAGS) $< -o $@
clean:
	rm $(TARGET)
	@for dir in $(SUBDIRS);\
		do $(MAKE) -C $$dir clean;\
	done
	