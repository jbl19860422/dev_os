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
	/*
	u8 tmp;
	tmp = in8(0x60);
	out8(0xa0,0x20);
	out8(0x20,0x20);
	static int int_count = 0;
	int_count++;
	PrintMsg("Get a interrupt intnum=");
	PrintDword(pt_regs.orig_eax&0xff);
	*/
	out8(0xa1, 0xff);
	static int count = 0;
	RGB rgb = {0xff, 0x00, 0x00};
	char *maskbuff_ascii = 0x200000;
	u8 flag;
	u8 deltx,delty;
	
	static u8 start = 0x00;
	if(start == 0xfa)
	{
		if(count == 0)
		{
			flag = in8(0x60);
		}
		else if(count == 1)
		{
			deltx = in8(0x60);
		}
		else if(count == 2)
		{
			static int mouse_x = 0;
			static int mouse_y = 0;
			delty = in8(0x60);
			int ideltx = deltx;
			if((flag &0x10)!=0)
			{
				ideltx|=0xffffff00;
			}
			int idelty = delty;
			if((flag &0x20)!=0)
			{
				idelty|=0xffffff00;
			}
			idelty = -idelty;
			mouse_x += ideltx;
			mouse_y += idelty;
			rec_t rec;
			rec.x0 = mouse_x;
			rec.y0 = mouse_y;
			DrawMouse(rec);
			
			char sFlag[3];
			char sDeltx[3];
			char sDelty[3];
			ctoax(flag, sFlag);
			ctoax(deltx, sDeltx);
			ctoax(delty, sDelty);	
			RGB white={0xff,0xff,0xff};
			DrawRec888(400, 300, 600, 320, white);
			DisplayStr(400, 300, sFlag, 0x200000, rgb);
			DisplayStr(450, 300, sDeltx, 0x200000, rgb);
			DisplayStr(500, 300, sDelty, 0x200000,rgb);
		}
		count++;
		count = count%3;
	}
	else
	{
		start = in8(0x60);
	}
	//in8(0x60);
	
	//DisplayStr(400, 300, data, 0x200000, rgb);
	//DisplayAscii(400, 200, 'I', maskbuff_ascii, rgb);
	//DisplayStr(400, 300, "abc", 0x200000, rgb);
	out8(0xa0, 0x20);
	out8(0x20, 0x20);
	out8(0xa1,0xef);
	
	/*
	if((pt_regs.orig_eax&0xff) == 0x2c)
	{//  Û±Í÷–∂œ
		u8 flag = in8(0x60);
		u8 deltx = in8(0x60);
		u8 delty = in8(0x60);
		char sFlag[3];
		char sDeltx[3];
		char sDelty[3];
		ctoax(flag, sFlag);
		ctoax(deltx, sDeltx);
		ctoax(delty, sDelty);
		
		RGB rgb = {0x00, 0xff, 0xff};
		DisplayStr(400, 300, sFlag, 0x200000, rgb);
		DisplayStr(450, 300, sDeltx, 0x200000, rgb);
		DisplayStr(500, 300, sDelty, 0x200000,rgb);
		out8(0xa0, 0x20);
		out8(0x20, 0x20);
	}
	*/
	//PrintMsg("count=");PrintDword(int_count);
	//PrintMsg("\r\n");
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
		"STI\n\t"\
		"IRET\n\t"
		:
		:
	);
}
