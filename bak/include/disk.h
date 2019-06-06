#ifndef __DISK_H__
#define __DISK_H__
#include "typedef.h"
typedef struct
{
	BYTE cPacketSize;
	BYTE cReserved;
	WORD uBlockCount;
	WORD uBufferOff;
	WORD uBufferSeg;
	WORD uBlockAddr0;
	WORD uBlockAddr1;
}DiskAddressPacket;
void ReadSector(WORD uBlockNum, WORD uBufferSeg, WORD uBufferOff,WORD uBlockAddr0, WORD uBlockAddr1);
#endif
