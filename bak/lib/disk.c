asm(".code16\n");
#include "disk.h"
void ReadSector(WORD uBlockNum, WORD uBufferSeg, WORD uBufferOff,WORD uBlockAddr0, WORD uBlockAddr1)
{	
	DiskAddressPacket stPack;
	stPack.cPacketSize = 0x10;
	stPack.cReserved = 0;
	stPack.uBlockCount = uBlockNum;
	stPack.uBufferSeg = uBufferSeg;
	stPack.uBufferOff = uBufferOff;
	stPack.uBlockAddr0 = uBlockAddr0;
	stPack.uBlockAddr1 = uBlockAddr1;
	__asm__ (
		"MOVW $0x00E0,%%DX\n\t"
                "MOVB $0x42,%%AH\n\t"
                "INT $0x13\n\t"
                "JC 1f\n\t"
                " #read ok\n\t"
                "JNC 2f\n\t"
                "1:\n\t"
                "MOVW $0,%%AX\n\t"
                "JMP 3f\n\t"
                "2:\n\t"
                "MOVW $1,%%AX\n\t"
                "3:\n\t"
                :
                :"S" (&stPack)
        );
	 __asm__ (
         "MOVL %%EBP,%%ESP\n\t"
         "POPL %%EBP\n\t"
           "RET"
           :
           :
        );
	
}
