#include "cdrom.h"
extern void switch_protect_to_real();
extern void switch_real_to_protect();
int checkInt13H()
{
	int iSupport = 0;
	__asm__ __volatile__(
		"MOVW $0x00E0,%%DX\n\t"
        	"MOVB $0x41,%%AH\n\t"
        	"MOVW $0x55AA,%%BX\n\t"
        	"INT $0x13\n\t"
        	"CMPW $0xAA55,%%BX\n\t"
        	"JNZ 1f   #no support\n\t"
        	"MOVL $0,%%EAX  #support\n\t"
		"JMP 2f \n\t"
		"1:\n\t"
        	"MOVL $-1,%%EAX\n\t"
		"2:\n\t"
		:"=&a"	(iSupport));
	return iSupport;
}

int ReadSector(DWORD dBlockAddr0, DWORD dBlockAddr1,DWORD wBlockNum, DWORD wBufferSeg, DWORD wBufferOff)
{
	int iRet = 0;
	DISK_ADDRESS_PACKET stPack;
	stPack.cPacketSize = 0x10;
	stPack.cReserved = 0;
	stPack.wBlockCount = wBlockNum;
	stPack.wBufferSeg = wBufferSeg;
	stPack.wBufferOff = wBufferOff;
	stPack.dBlockAddr0 = dBlockAddr0;
	stPack.dBlockAddr1 = dBlockAddr1;
	//PrintMsg("start ReadSector1\r\n");
	__asm__ __volatile__ (
			"CALL switch_protect_to_real\n\t"
			".code16\n\t"
			"PUSHW %%DS\n\t"
			"MOVL %%ESI,%%EAX\n\t"
			"SHRL $4,%%EAX\n\t"
			"ANDW $0xFFFF,%%AX\n\t"
			"MOVW %%AX,%%DS\n\t"
			"ANDW $0x000F, %%SI\n\t"
			"MOVB $0x42,%%AH\n\t"
			"INT $0x13\n\t"
			"JC 1f\n\t"
			"JNC 2f\n\t"
			"1:\n\t"
			"MOVW $-1,%%BX\n\t"
			"JMP 3f\n\t"
			"2:\n\t"
			"MOVW $0,%%BX\n\t"
			"3:\n\t"
			"POPW %%DS\n\t"
			"PUSH %%CS\n\t"
			"CALL switch_real_to_protect\n\t"
			".code32\n\t"
			"NOP\n\t"
			:"=b" (iRet)
			:"S" (&stPack), "d" (g_iDriverNum)
        );
	//PrintMsg("end readsector\r\n");
	return iRet;
}



