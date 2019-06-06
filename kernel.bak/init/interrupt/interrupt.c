#include "interrupt.h"
#include "typedefs.h"
void asmlinkage IRQ_PROCESS(0x20);
void asmlinkage IRQ_PROCESS(0x21);
void asmlinkage IRQ_PROCESS(0x22);
void asmlinkage IRQ_PROCESS(0x23);
void asmlinkage IRQ_PROCESS(0x24);
void asmlinkage IRQ_PROCESS(0x25);
void asmlinkage IRQ_PROCESS(0x26);
void asmlinkage IRQ_PROCESS(0x27);
void asmlinkage IRQ_PROCESS(0x28);
void asmlinkage IRQ_PROCESS(0x29);
void asmlinkage IRQ_PROCESS(0x2A);
void asmlinkage IRQ_PROCESS(0x2B);
void asmlinkage IRQ_PROCESS(0x2C);
void asmlinkage IRQ_PROCESS(0x2D);
void asmlinkage IRQ_PROCESS(0x2E);
void asmlinkage IRQ_PROCESS(0x2F);

void common_interrupt(pt_regs_t pt_regs)
{
	u8 tmp;
	tmp = in8(0x60);
	out8(0x20,0x20);
	out8(0xa0,0x20);
	static int int_count = 0;
	int_count++;
	PrintMsg("Get a interrupt intnum=");
	PrintDword(pt_regs.orig_eax&0xff);
	PrintMsg("count=");PrintDword(int_count);
	PrintMsg("\r\n");
}

void common_ret_from_int(pt_regs_t pt_regs)
{
	__asm__
	(
		"MOVB $0x20,%%AL\n\t"
		"OUTB %%AL,$0x20\n\t"
		"ADDL $4,%%ESP\n\t"
		"POPL %%EBX\n\t"\
		"POPL %%ECX\n\t"\
		"POPL %%EDX\n\t"\
		"POPL %%ESI\n\t"\
		"POPL %%EDI\n\t"\
		"POPL %%EBP\n\t"\
		"POPL %%EAX\n\t"\
		"POPL %%DS\n\t"\
		"POPL %%ES\n\t"\
		"ADDL $4,%%ESP\n\t"\
		"IRET\n\t"
		:
		:
	);
}
