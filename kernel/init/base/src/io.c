#include "io.h"
void WritePortLong(short sPort, DWORD data)
{
        __asm__ (
                "OUTL %%EAX,%%DX\n\t"
                :
                :"a" (data), "d" (sPort)
        );
}
long ReadPortLong(short sPort)
{
        long iData;
        __asm (
                "INL %%DX,%%EAX\n\t"
                :"=&a" (iData)
                :"d" (sPort)
        );
	return iData;
}

void WritePortShort(short sPort, WORD data)
{
        __asm__ (
                "OUT %%AX,%%DX\n\t"
                :
                :"a" (data), "d" (sPort)
        );
}
short ReadPortShort(short sPort)
{
        short sData;
        __asm (
                "INL %%DX,%%EAX\n\t"
                :"=&a" (sData)
                :"d" (sPort)
        );
	return sData;
}

void WritePortChar(short sPort, char data)
{
        __asm__ (
                "OUTB %%AL,%%DX\n\t"
                :
                :"a" (data), "d" (sPort)
        );
}
char ReadPortChar(short sPort)
{
        char data;
        __asm (
                "INB %%DX,%%AL\n\t"
		"MOV $0, %%AH\n\t"
                :"=&a" (data)
                :"d" (sPort)
        );
	return data;
}

void out8(u16 port, u8 data)
{
	__asm__ 
	(
		"OUTB %%AL,%%DX\n\t"
		:
		:"a" (data), "d" (port)
    );
}

void out16(u16 port, u16 data)
{
	__asm__ 
	(
		"OUTW %%AX,%%DX\n\t"
		:
		:"a" (data), "d" (port)
    );
}

void out32(u16 port, u32 data)
{
	__asm__ 
	(
		"OUTL %%EAX,%%DX\n\t"
		:
		:"a" (data), "d" (port)
    );
}

u8 in8(u16 port)
{
	u8 data;
	__asm__
	(
		"XORL %%EAX,%%EAX\n\t"
		"INB %%DX,%%AL\n\t"
		:"=&a" (data)
		:"d" (port)
	);
	return data;
}

u16 in16(u16 port)
{
	u16 data;
	__asm__
	(
		"XORL %%EAX,%%EAX\n\t"
		"INW %%DX,%%AX\n\t"
		:"=&a" (data)
		:"d" (port)
	);
	return data;
}

u32 in32(u16 port)
{
	u32 data;
	__asm__
	(
		"XORL %%EAX,%%EAX\n\t"
		"INL %%DX,%%EAX\n\t"
		:"=&a" (data)
		:"d" (port)
	);
	return data;
}

void io_delay()
{
	__asm__
	(
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		::
	);
}




