asm(".code16gcc");

typedef char BYTE;
typedef short WORD;
typedef int DWORD;
int ReadSector(WORD uBlockNum, WORD uBufferSeg, WORD uBufferOff,int uBlockAddr0, int uBlockAddr1);
#define IN 
#define OUT
#define INOUT
int iSystemSize = 0;
WORD iDriverNum;
extern int DisplayMsg(const char *msg);
void DisplayChar(char cData);
void DisplayWord(short sData);
void DisplayCharNum(char cData);
void DisplayInt(int iData);
void memcpy(char *dst, char *src, int iSize)
{
	DisplayMsg("\r\nsrc=");
	DisplayInt((int)src);
	DisplayMsg("\r\ndst=");
	DisplayInt((int)dst);
	int i=0;
	for(;i<iSize;i++)
	{
		dst[i] = src[i];
	}
}
#pragma pack(1)
typedef struct
{
        BYTE cPacketSize;
        BYTE cReserved;
        WORD uBlockCount;
        WORD uBufferOff;
        WORD uBufferSeg;
        int uBlockAddr0;
        int uBlockAddr1;
}DiskAddressPacket;
typedef struct
{
	char cDirRecLen;
	char cExtAttrRecLen;
	int iLocOfExtBig;
	int iLocOfExtSmall;
	DWORD iDataLenBig;
	DWORD iDataLenSmall;
	char sRecDateTime[7];
	BYTE cFileFlags;
	BYTE cFileUnitSize;
	BYTE cGapSize;
	DWORD iVolSeqNum;
	BYTE cFileIdenLen;
	BYTE sFileName[256];
}FDDescriptor;
#pragma pack()
void WritePort(short sPort, DWORD data)
{
	__asm__ (
		"OUTL %%EAX,%%DX\n\t"
		:
		:"a" (data), "d" (sPort)
	);
}
int ReadPort(short sPort)
{
	int iData;
	__asm (
		"INL %%DX,%%EAX\n\t"
		:"=&a" (iData)
		:"d" (sPort)
	);
}
#pragma pack(1)
typedef struct
{
	WORD sVenderID;
	WORD sDeviceID;
	WORD sCommand;
	WORD sStatus;
	char cRevision;
	char sClassCode[3];
	char cCacheLineSize;
	char cLatencyTimer;
	char cHeaderType;
	char cBIST;	
}TPCIConfigHead;
typedef struct 
{
	BYTE cVolType;
	BYTE sStdIdentifier[5];
	BYTE cVolVer;
	BYTE cUnused1;
	BYTE sSysIdentifier[32];
	BYTE sVolIdentifier[32];
	BYTE sUnused2[8];
	DWORD iVolSpaceSizeBig;
	DWORD iVolSpaceSizeSmall;
	BYTE sUnused3[32];
	DWORD iVolSetSize;
	DWORD iVolSeqNum;
	DWORD iBlockSize;
	DWORD iPathTabSizeBig;
	DWORD iPathTabSizeSmall;
	DWORD iLPathTablePos;
	DWORD iLPathTableOpPos;
	DWORD iMPathTablePos;
	DWORD iMPathTabOpPos;
	BYTE RootDirRec[34];
	//others ignore
}VolumeDescriptor;
#pragma pack()
void checkInt13H()
{
	short iSupport = 0;
	__asm__ __volatile__(
		"MOVW $0x00E0,%%DX\n\t"
        	"MOVB $0x41,%%AH\n\t"
        	"MOVW $0x55AA,%%BX\n\t"
        	"INT $0x13\n\t"
        	"CMPW $0xAA55,%%BX\n\t"
        	"JNZ 1f   #no support\n\t"
        	"MOVW $1,%%AX  #support\n\t"
		"JMP 2f \n\t"
		"1:\n\t"
        	"MOVW $0,%%AX\n\t"
		"2:\n\t"
		:"=&a"	(iSupport));
	
	if(iSupport==1)
	{
		DisplayMsg("support yes\r\n");
	}
	else
	{
		DisplayMsg("not support yes\r\n");
	}
}
//DiskAddressPacket stPack;
char buffer[2048];
int FindFile(char *sFileName,FDDescriptor* fd)
{
	int iRet = 0;
	buffer[0] = 0xaa;
  	iRet = ReadSector(1,0,(WORD)buffer,16,0x0);
        FDDescriptor *pRootDirRec = ((VolumeDescriptor *)buffer)->RootDirRec;
        int iRootSector = pRootDirRec->iLocOfExtBig;
	DisplayInt(iRootSector);
	ReadSector(1,0,(WORD)buffer,iRootSector,0x0);
	FDDescriptor *pRec = (FDDescriptor *)buffer;
	char cLen= pRec->cDirRecLen;
	int iFindKernel = 0;
	DisplayMsg("Dir Len:");
	DisplayCharNum(cLen);
	DisplayMsg("\r\n");
	while(cLen>0)
	{
		DisplayMsg(pRec->sFileName);	
		if(0==CmpStr(pRec->sFileName, sFileName))
	        {
			iFindKernel = 1;
			DisplayMsg("\r\nfind kernel\r\n");
			DisplayMsg("msg\r\n");
			DisplayInt((int)pRec);
			*fd = *pRec;
			break;
        	}
		pRec = (FDDescriptor*)((char *)pRec+cLen);
		cLen = pRec->cDirRecLen;
	}
	if(iFindKernel == 1)
	{
		char *msg="find kernel\n";
		DisplayMsg(msg);
	}
	else
	{
		char *msg1="not find kernel\n";
		DisplayMsg(msg1);
	}
	__asm__ (
                "MOVL %%EBP,%%ESP\n\t"
                "POPL %%EBP\n\t"
                "POPW %%BX\n\t"
		"MOVW $1,%%AX\n\t"
                "JMP %%BX\n\t"
                :
                :
        );
	
	return 1;
}

int CmpStr(char *pSrc, char *pDst)
{
	int i = 0;
	while(pSrc[i] != '\0' && pDst[i] != '\0')
	{
		if(pSrc[i] != pDst[i])
		{
			return -1;
		}
		i++;
	}
	if(pSrc[i] == '\0' && pDst[i] == '\0')
	{
		return 0;
	}
	else
	{
		return -2;
	}	
}
	
int ReadSector(WORD uBlockNum, WORD uBufferSeg, WORD uBufferOff,int uBlockAddr0, int uBlockAddr1)
{
	int iRet = 0;
	DiskAddressPacket stPack;
        stPack.cPacketSize = 0x10;
        stPack.cReserved = 0;
        stPack.uBlockCount = uBlockNum;
        stPack.uBufferSeg = uBufferSeg;
        stPack.uBufferOff = uBufferOff;
        stPack.uBlockAddr0 = uBlockAddr0;
        stPack.uBlockAddr1 = uBlockAddr1;
	__asm__ __volatile__ (
		"PUSHW %%DS\n\t"
		"MOVW %%SS,%%AX\n\t"
		"MOVW %%AX,%%DS\n\t"
                "MOVB $0x42,%%AH\n\t"
                "INT $0x13\n\t"
                "JC 1f\n\t"
                "JNC 2f\n\t"
                "1:\n\t"
                "MOVW $-1,%%AX\n\t"
                "JMP 3f\n\t"
                "2:\n\t"
                "MOVW $0,%%AX\n\t"
                "3:\n\t"
		"POPW %%DS\n\t"
                :"=&a" (iRet)
                :"S" (&stPack), "d" (iDriverNum)
        );
	return iRet;
}
void ReadPCIConfig()
{
	int iBus = 0;
	int iDevice = 0;
	int iTmp;
	int iData = 0;  
	int iData1 = 0;
	char *smsg="Device Vender Base Class\r\n";
	char *sRet = "\r\n";
	int iFun = 0;
	DisplayMsg(smsg);
	for(;iBus<256;iBus++)
	{
		iDevice = 0;
		for(;iDevice<32;iDevice++)
		{
			iFun = 0;
			for(;iFun<8;iFun++)
			{
				iData = 0xffff;
				iTmp = (0x80<<24)|(iBus&0xff)<<16|(iDevice&0x1f)<<11|(iFun&0xb)<<8;
				WritePort(0xcf8, iTmp);
				iData = ReadPort(0xcfc);
                                iTmp = (0x80<<24)|(iBus&0xff)<<16|(iDevice&0x1f)<<11|(iFun&0xb)<<8|2<<2;
				WritePort(0xcf8, iTmp);
                                iData1=ReadPort(0xcfc);

				if((iData&0xffff)!=0xffff)
				{
					DisplayWord((short)((iData>>16)&0xffff));
					DisplayMsg("   ");
					DisplayWord((short)(iData&0xffff));					       			 DisplayMsg("   ");
                                	DisplayCharNum((char)(iData1>>24&0xff));
                                	DisplayMsg("   ");
                                	DisplayWord((short)((iData1>>8)&0xffff));
                                	DisplayMsg("\r\n");
					iData = 0;
				}
				else
				{
					continue;
				}
			}
		}
	}
	__asm__ (
                "MOVL %%EBP,%%ESP\n\t"
                "POPL %%EBP\n\t"
                "POPW %%BX\n\t"
                "JMP %%BX\n\t"
                :
                :
        );
}
void DisplayWord(short sData)
{
	char cDataHigh = (char)((sData>>8)&0xFF);
	DisplayCharNum(cDataHigh);
	char cDataLow = (char)((sData&0xFF));
	DisplayCharNum(cDataLow);
}

void DisplayInt(int iData)
{
	short sHigh = (iData>>16)&0xFFFF;
	short sLow = (iData)&0xFFFF;
	DisplayWord(sHigh);
	DisplayWord(sLow);
}
void DisplayCharNum(char cData)
{
	char cTmp;
	char cHigh = (cData>>4)&0x0F;
	if(cHigh<10)
        {
                cTmp = cHigh+'0';
        }
        else
        {
                cTmp = cHigh+'A'-10;
        }
	DisplayChar(cTmp);
	char cLow = cData&0x0F;
        if(cLow<10)
        {
                cTmp = cLow+'0';
        }
        else
        {
                cTmp = cLow+'A'-10;
        }
	DisplayChar(cTmp);
	
}

void DisplayChar(char cData)
{
	__asm__ (
	"MOVW $7,%%BX\n\t"
	"MOVB $0xe,%%AH\n\t"
	"INT $0x10"
	:
	:"a" (cData)
	);
}

typedef struct
{
	int iBaseAddrLow;
	int iBaseAddrHigh;
	int iLengthLow;
	int iLengthHigh;
	int iType;
}ARDS; 
ARDS vArds[100];
void GetMemConfig()
{
	char *msg_ret="\n";
	DisplayMsg(msg_ret);
	int iRet = 0;
	int iMemConfNum = 0;
	int iReadMemNotEnd = 1;
	ARDS *pMbh = vArds;
	__asm__ (
	"MOVL $0,%%EBX\n\t"
	:
	:
	);
	while(iReadMemNotEnd)
	{
		__asm__ (
        	"MOVL $0xe820,%%EAX\n\t"
        	"MOVL $0x20,%%ECX\n\t"
        	"MOVL $0x534d4150,%%EDX\n\t"
        	"INT $0x15\n\t"
		"MOVL $0,%%EAX\n\t"
        	"JC 1f\n\t"
		"JMP 2f\n\t"
		"1:\n\t"
		"MOVL $1,%%EAX\n\t"
		"2:\n\t"
		:"=b" (iReadMemNotEnd),"=a" (iRet) 
		:"D" (pMbh)
		);
		if(iRet != 0)
		{
			break;
		}
		pMbh++;
		iMemConfNum++;
	}
	if(iRet != 0)
	{
		char *msg = "GetMemConf Err\n";
		DisplayMsg(msg);	
	}
	else
	{
		pMbh =vArds;
		int i=0;
		int j=0;
		for(i=0;i<iMemConfNum;i++)
		{
			for(j=0;j<5;j++)
			{
				DisplayInt(*((int *)pMbh+j));
				if(j!=5)
	                        	DisplayMsg("   ");
			}	
			pMbh++;
			char *msg2="\r\n";
			DisplayMsg(msg2);
		}
	}
	 __asm__ (
                "MOVL %%EBP,%%ESP\n\t"
                "POPL %%EBP\n\t"
                "POPW %%AX\n\t"
                "JMP %%AX\n\t"
                :
                :
        );

}
//FDDescriptor fd;
int iBigMode = 0;
void DetectBig()
{
	short sTmp=0xaa55;
	char *p=(char *)(&sTmp);
	if(p[0] == 0x55)
	{
		iBigMode = 0;
		DisplayMsg("Small Mode");
	}
	else
	{
		iBigMode = 1;
		DisplayMsg("Big Mode");
	}
	 __asm__ (
                "MOVL %%EBP,%%ESP\n\t"
                "POPL %%EBP\n\t"
                "POPW %%AX\n\t"
                "JMP %%AX\n\t"
                :
                :
        );
}
void lmain(void)
{
	char *fileName="KERNEL.BIN";
	FDDescriptor fd;
	DetectBig();
	DisplayMsg("\r\n");
	DisplayMsg("DriverNum:");
	DisplayWord(iDriverNum);
	DisplayMsg("\r\n");
	//GetMemConfig();
	DisplayMsg("\r\nfd addr=0x");
	DisplayInt((int)&fd);
	DisplayMsg("\r\n");
	FindFile(fileName,&fd);
	//the return code is not right run in bochs
	//so i use jump instruction to jump back to
	//the right address(after call main)
	//ReadPCIConfig();	
	DisplayMsg("\r\nafdasfsdafas");
	DisplayMsg("\r\nSystemSize:");
	DisplayInt((int)&iSystemSize);
	DisplayMsg("\r\nKernel at:");
	int iKernelAddr0 = fd.iLocOfExtBig;
	DisplayInt(*(int *)((char *)&fd+2));//16bit compile always has err,so..	
	//int ReadSector(WORD uBlockNum, WORD uBufferSeg, WORD uBufferOff,int uBlockAddr0, int uBlockAddr1);
	int iKernelBlockNum = fd.iDataLenBig;
	iKernelBlockNum=iKernelBlockNum/2048+1;
	DisplayMsg("\r\nKernel size=");
	DisplayInt(iKernelBlockNum);
	iSystemSize = (int)&iSystemSize;
	iSystemSize = (iSystemSize/1024+1)*1024;
	DisplayMsg("\r\nKernel load at 0x0:");
	DisplayInt(iSystemSize);
	DisplayMsg("\r\n");
	DisplayMsg("DriverNum:");
	DisplayWord(iDriverNum);
	DisplayMsg("\r\n");
	DisplayMsg("iKernelBlockNum:");
	DisplayInt(iKernelBlockNum);
	DisplayMsg("\r\n");	
	int iRet = ReadSector(iKernelBlockNum, 0, iSystemSize, iKernelAddr0, 0);
	if(0 == iRet)
	{
		DisplayMsg("\r\nLoad Kernel Ok\r\n");
	}
	else
	{
		DisplayMsg("Load Kernel Fail\r\n");
	}
	__asm__ (
                "MOVL %%EBP,%%ESP\n\t"
                "POPL %%EBP\n\t"
                "POPW %%AX\n\t"
                "JMP %%AX\n\t"
                :
                :
        );

}


