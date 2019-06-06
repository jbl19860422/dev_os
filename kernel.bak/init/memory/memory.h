#ifndef _MEMORY_H_
#define _MEMORY_H_
#include "typedefs.h"
#include "list.h"
extern int _end;
#pragma pack(1)
typedef struct
{
	DWORD dBaseAddrLow;
	DWORD dBaseAddrHigh;
	DWORD dLengthLow;
	DWORD dLengthHigh;
	DWORD dType;
}ARDStructure;

typedef struct page_t
{
	struct list_head head;
	unsigned long address;
	int iUsed;
	int iMapNr;
};

typedef struct free_area_t
{
	struct list_head head;
};

struct free_area_t free_areas[10];

void InitFreeArea();
#pragma pack()
#define MAX_MEM_CONF 0x10
#define ARDTYPE_USABLE 1
#define ARDTYPE_RESERVED 2
//extern int _end;
int g_iMemConfCount;
ARDStructure g_memConf[MAX_MEM_CONF];
struct page_t *g_pMemMap;
char *g_pMallocStart;
int g_iMapNr;
void ReadMemConf();
void InitPageConf();
//@function: GetFreePage from low memory 1m
//@param[in]: iCount--how many pages to get
//@param[out]: the malloc address 
//@return: 0--success,not 0--fail
int GetFreePage(int iCount);//ªÒ»°ø’œ–“≥√Ê

//@function: FreePage
//@param[in] pAddr--the address
//@param[in] iCount--the page count
void FreePage(void *pAddr);

void DecodeAddrTo16Mode(int addr, short *seg, short *off);

int GetMapAddr(int addr);

void memcpy(char *dst, char *src, int iSize);
#endif
