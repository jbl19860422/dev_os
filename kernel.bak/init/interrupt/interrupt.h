#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_
extern int SAVEALL;
#define asmlinkage __attribute((regparm(0)))
#define IRQ_PROCESS_TMP(nr) int_process##nr
#define IRQ_PROCESS(nr) IRQ_PROCESS_TMP(nr)(void) \
{\
	__asm__\
	(\
		"CLI\n\t"\
		"ADDL $4,%%ESP\n\t"\
		"PUSHL $"#nr"-256\n\t"\
		"PUSHL %%ES\n\t"\
		"PUSHL %%DS\n\t"\
		"PUSHL %%EAX\n\t"\
		"PUSHL %%EBP\n\t"\
		"PUSHL %%EDI\n\t"\
		"PUSHL %%ESI\n\t"\
		"PUSHL %%EDX\n\t"\
		"PUSHL %%ECX\n\t"\
		"PUSHL %%EBX\n\t"\
		"PUSHL $common_ret_from_int\n\t"\
		"STI\n\t"\
		"JMP common_interrupt\n\t"\
		:\
		:\
	);\
}

typedef struct
{
	long ebx;
	long ecx;
	long edx;
	long esi;
	long edi;
	long ebp;
	long eax;
	int xds;
	int xes;
	long orig_eax;
	long eip;
	int xcs;
	long eflags;
	long esp;
	int xss;
}pt_regs_t;
#endif
