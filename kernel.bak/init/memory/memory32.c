#include "memory.h"
int g_iMapNr = 0;
int g_iMemConfCount = 0;
char *g_pMallocStart = NULL;
ARDStructure g_memConf[MAX_MEM_CONF];
void ReadMemConf()
{
	int i = 0;
	int iReadMemConfError = 0;
	int iEBX = 0;
	BOOL bNotReadMemConfEnd = TRUE;
	//ARDStructure *pARD = g_memConf;
	g_iMemConfCount = 0;
	
	while(!iReadMemConfError)//读取内存配置无错误，并且未结束
	{
		__asm__ __volatile__ (
			"CALL switch_protect_to_real\n\t"
			".code16\n\t"
			"PUSHW %%ES\n\t"
			"MOVL %%EDI,%%EAX\n\t"
			"SHRL $4, %%EAX\n\t"
			"MOVW %%AX,%%ES\n\t"
			"ANDW $0x0F,%%DI\n\t"
			"MOVL $0xe820,%%EAX\n\t"
			"MOVL $0x534d4150,%%EDX\n\t"
			"MOVL $20,%%ECX\n\t"
		"1:\n\t"
			"INT $0x15\n\t"
			"JC ReadMemConfError\n\t"
			"MOVL $0,%%EDX\n\t"
			"JMP 2f\n\t"
		"ReadMemConfError:\n\t"
			"MOVL $-1,%%EDX\n\t"
		"2:\n\t"
			"POPW %%ES\n\t"
			"PUSHW %%CS\n\t"
			"CALL switch_real_to_protect\n\t"
			".code32\n\t"
			"NOP\n\t"
			:"=b" (iEBX), "=d" (iReadMemConfError)
			:"D" (&g_memConf[g_iMemConfCount]), "b" (iEBX)
		);
		g_iMemConfCount++;
		if(iEBX == 0)
		{
			break;
		}
		//pARD++;
	}
}

void InitPageConf()
{
	//读取内存配置完成，接下来将_end后面的内存用作可分配的内存进行配置
	struct page_t *pMemMap;
	g_pMemMap = (struct page *)(((int)(&_end)/4096)*4096+4096);/*4k对齐*/
	pMemMap = g_pMemMap;
	int iCount = 0;
	int i = 0;
	int j = 0;
	PrintMsg("BaseLow     BaseHigh     LengthLow     LengthHigh     Type\r\n");	
	for(i = 0; i < g_iMemConfCount; i++)
	{
		if(g_memConf[i].dBaseAddrHigh == 0 && g_memConf[i].dLengthHigh == 0)
		{
			if(g_memConf[i].dBaseAddrLow <= 0xFFFFF && g_memConf[i].dBaseAddrLow+g_memConf[i].dLengthLow <= 0xFFFFF)//1m 以内
			{
				PrintDword(g_memConf[i].dBaseAddrLow);
				PrintMsg("  ");
				PrintDword(g_memConf[i].dBaseAddrHigh);
				PrintMsg("   ");
				PrintDword(g_memConf[i].dLengthLow);
				PrintMsg("   ");
				PrintDword(g_memConf[i].dLengthHigh);
				PrintMsg("   ");
				PrintDword(g_memConf[i].dType);
				PrintMsg("\r\n");
			}
		}
	}
	for(i = 0; i < g_iMemConfCount; i++)
	{
		if(g_memConf[i].dBaseAddrHigh == 0 && g_memConf[i].dLengthHigh == 0)
		{
			if(g_memConf[i].dBaseAddrLow <= 0xFFFFF && g_memConf[i].dBaseAddrLow+g_memConf[i].dLengthLow <= 0xFFFFF)//1m 以内
			{
				if(g_memConf[i].dType == ARDTYPE_USABLE)
				{
					int iPageCount = g_memConf[i].dLengthLow/4096;//页数
					for(j=0;j<iPageCount;j++)
					{
						if((g_memConf[i].dBaseAddrLow+j*4096>=0xA0000&&g_memConf[i].dBaseAddrLow+j*4096<=0xFFFFF)||g_memConf[i].dBaseAddrLow+j*4096 <= &_end)
						{
							continue;
						}
						pMemMap->address = g_memConf[i].dBaseAddrLow+j*4096;
						pMemMap->iMapNr = g_iMapNr++;
						pMemMap->iUsed = 0;
						INIT_LIST_HEAD(&(pMemMap->head));
						pMemMap++;
					}
				}
			}
		}
	}
	//接下来，将mem_map用掉的内存页面设置为已使用
	for(i=0;i<g_iMapNr;i++)
	{
		struct page_t *pPage = GetMapAddr(&g_pMemMap[i]);
		pPage->iUsed = 1;
	}
	/*
	PrintMsg("\r\nmapaddr=");
	PrintDword((int)g_pMemMap);
	PrintMsg("&end=");
	PrintDword((int)&_end);
	*/
	g_pMallocStart = ((int)pMemMap/4096)*4096+4096;
	InitFreeArea();
	
	int address = GetFreePage(2);
	FreePage(address);
	PrintMsg("\r\nthe get address is ");
	PrintDword(address);
	PrintMsg("\r\n");
	PrintMsg("the start map addr is ");
	PrintDword(GetMapAddr(address));
	PrintMsg("\r\n");
}

void InitFreeArea()
{
	
}
int GetMapAddr(addr)
{
	int i= 0;
	for(i=0; i < g_iMapNr; i++)
	{
		if(g_pMemMap[i].address == addr)
		{
			return &g_pMemMap[i];
		}
	}
	return NULL;
}

int GetFreePage(int iPageCount)
{
	//循环查找没被使用的页面
	int i = 0;
	int iCount = 0;
	int iAddr = 0;
	struct list_head *pPageHead;
	struct list_head *pPageNext;
	for(i = 0; i < g_iMapNr; i++)
	{
		if(g_pMemMap[i].iUsed != 1)
		{
			if(iAddr == 0)
			{
				iAddr = g_pMemMap[i].address;
				pPageHead = &(g_pMemMap[i].head);
			}
			else
			{
				list_add_tail(&(g_pMemMap[i].head), pPageHead);
			}
			g_pMemMap[i].iUsed = 1;
			iCount++;
			if(iCount >= iPageCount)
			{
				break;
			}
		}
	}
	if(iCount < iPageCount)
	{// 回滚之前分配的
		pPageNext = pPageHead->next;
		while(pPageNext->next != pPageHead)
		{
			list_del(pPageNext);
			struct page_t *pTmp = list_entry(pPageNext, struct page_t, head);
			pTmp->iUsed = 0;
			pPageNext = pPageHead->next;
		}
		((struct page_t *)list_entry(pPageHead, struct page_t, head))->iUsed = 0;
		INIT_LIST_HEAD(pPageHead);
		return NULL;
	}
	return iAddr;
}

void FreePage(void *pAddr)
{
	struct list_head *pPageHead = &(((struct page_t *)GetMapAddr((int)pAddr))->head);
	if(pPageHead == NULL)
	{
		return;
	}
	struct list_head *pNextPage = pPageHead->next;
	while(pNextPage != pPageHead)
	{
		list_del(pNextPage);
		((struct page_t *)list_entry(pNextPage, struct page_t, head))->iUsed = 0;
		pNextPage = pPageHead->next;
	}
	((struct page_t *)list_entry(pPageHead, struct page_t, head))->iUsed = 0;
	INIT_LIST_HEAD(pPageHead);
}

void DecodeAddrTo16Mode(int addr, short *seg, short *off)
{
	*seg = (addr>>4)&0xffff;
	*off = addr&0x0f;
}

void memcpy(char *dst, char *src, int iSize)
{
	int i=0;
	for(;i<iSize;i++)
	{
		dst[i] = src[i];
	}
}
