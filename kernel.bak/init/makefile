SUBDIRS	:=	ata driver memory interrupt
SUBOBJS := ata/ata.o driver/driver.o base/base.o memory/memory.o interrupt/interrupt.o interrupt/interrupt_entry.o
TARGET	:= 	init.o
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

CUR_DIR=$(TOPDIR)/init
BASE=$(CUR_DIR)/base
DRIVER=${CUR_DIR}/driver
ATA=$(CUR_DIR)/ata
MEMORY=$(CUR_DIR)/memory
INTERRUPT=$(CUR_DIR)/interrupt

BASE_INC=-I${BASE}/inc
MEMORY_INC=-I${MEMORY}
ATA_INC=-I${ATA}
PCI_INC=-I${DRIVER}/pci
8259_INC=-I${DRIVER}/8259
CDROM_INC=-I$(DRIVER)/cdrom
VGA_INC=-I$(DRIVER)/vga
INTR_INC=-I${INTERRUPT}

ALL_INC=$(BASE_INC) $(MEMORY_INC) $(ATA_INC) $(PCI_INC) $(8259_INC) $(INTR_INC) $(CDROM_INC) $(VGA_INC) -I$(CUR_DIR)
all: $(TARGET)
$(TARGET):$(SUBDIRS) $(OBJS)
	@for dir in $(SUBDIRS);\
		do $(MAKE) -C $$dir all CC=$(CC) AR=$(AR) PARENT_DIR=$(PWD) ALL_INC="${ALL_INC}"; \
	done
	$(LD) $(LD_FLAGS) $(OBJS) $(SUBOBJS) -o $@.o;
	objcopy	-S -O binary $(TARGET).o $@;
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