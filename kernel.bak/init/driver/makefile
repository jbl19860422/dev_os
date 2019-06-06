SUBDIRS	:=	pci 8259 cdrom vga
SUBOBJS := pci/pci.o 8259/8259.o cdrom/cdrom32.o cdrom/cdrom16.o vga/vga.o
TARGET	:= 	driver.a
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
LD_FLAGS := 

CUR_DIR=$(PARENT_DIR)/driver

all: $(TARGET)
$(TARGET):$(SUBDIRS) $(OBJS)
	cd $(CUR_DIR)
	@for dir in $(SUBDIRS);\
		do $(MAKE) -C $$dir all CC=$(CC) AR=$(AR) PARENT_DIR=$(PWD) ALL_INC="${ALL_INC}"; \
	done
	$(AR) $(AR_FLAGS) $(OBJS) $(SUBOBJS) -o $@;
.s.o:%.s
	$(ASM) $(ASM_FLAGS) $< -o $@
.c.o:%.c
	@echo $(ALL_INC)
	$(CC) $(CC_FLAGS) $(ALL_INC) $< -o $@
clean:
	rm $(TARGET)
	@for dir in $(SUBDIRS);\
		do $(MAKE) -C $$dir clean;\
	done