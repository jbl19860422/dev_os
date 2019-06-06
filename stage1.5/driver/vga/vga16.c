#include "vga.h"
asm(".code16gcc");
extern void DisplayInt(int a);
void PrintChar16(int iRow, int iCol, char cColor, char cChar)
{
	WORD wTmp;
	*((char *)&wTmp) = cChar;
	*((char *)&wTmp+1) = cColor;
	__asm__ (
		"PUSHW %%DS\n\t"
		"MOVL %%EDI,%%EAX\n\t"
		"SHRL $4,%%EAX\n\t"
		"MOVW %%AX,%%DS\n\t"
		"ANDW $0x0F,%%DI\n\t"
		"MOVW %%DX,(%%DI)\n\t"
		"POPW %%DS\n\t"
		:
		:"D" (&vVGA[VGA_LINE_COUNT*iRow+iCol]), "d" (wTmp)
	);
    //vVGA[VGA_LINE_COUNT*iRow+iCol] = wTmp;
}

void PrintMsg16(char *sMsg)
{
	int i = 0;
	while(sMsg[i] != '\0')
	{	
		if(sMsg[i] == '\r')
		{
			iCurCol = 0;
			i++;
			continue;
		}
		else if(sMsg[i] == '\n')
		{
			iCurRow++;
			i++;
			continue;
		}
		PrintChar16(iCurRow, iCurCol, 0xc, sMsg[i]);
		iCurCol++;
		if(iCurCol%80 == 0)
		{
			iCurCol = 0;
			iCurRow++;
		}
		i++;
	}
	
	return;
}