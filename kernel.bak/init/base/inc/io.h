#ifndef _IO_H_
#define _IO_H_
#include "typedefs.h"
#include "io.h"
void WritePortLong(short sPort, DWORD data);
void WritePortShort(short sPort, WORD data);
void WritePortChar(short sPort, char data);
long ReadPortLong(short sPort);
short ReadPortShort(short sPort);
char ReadPortChar(short sPort);

void out32(u16 port, u32 data);
void out16(u16 port, u16 data);
void out8(u16 port, u8 data);
u32 in32(u16 port);
u16 in16(u16 port);
u8 in8(u16 port);

void io_delay();

#endif

