asm(".code16gcc");
typedef char BYTE;
typedef short WORD;
typedef int DWORD;
int ReadSector(WORD uBlockNum, WORD uBufferSeg, WORD uBufferOff,int uBlockAddr0, int uBlockAddr1);

extern int DisplayMsg(const char *msg);
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
	BYTE cDirRecLen;
	BYTE cExtAttrRecLen;
	DWORD iLocOfExtBig;
	DWORD iLocOfExtSmall;
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
	//char buffer[2048];
	__asm__ (
	"MOVW $0xABCD,%%AX\n\t"
	:
	:
	);
  	ReadSector(1,0x7c0,buffer,16,0x0);
	VolumeDescriptor *pDesc = (VolumeDescriptor *)buffer;
        FDDescriptor *pRootDirRec = pDesc->RootDirRec;
        int iRootSector =*((int *)((char *)pRootDirRec+2));
        __asm__ (
        "MOVW $0x55AA,%%BX\n\t"
        :
        :"S" (iRootSector)
        );
	
	ReadSector(1,0x7c0,buffer,iRootSector,0x0);
	FDDescriptor *pRec = buffer;
	char cLen= pRec->cDirRecLen;
	int iFindKernel = 0;
	
	while(cLen>0)
	{	
		if(0==CmpStr((char *)pRec+33, sFileName))
	        {
			iFindKernel = 1;
			break;
        	}
		pRec = (char *)pRec+cLen;
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
	int iRet;
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
                "MOVW $0x00E0,%%DX\n\t"
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
                :"S" (&stPack)
        );
	return iRet;
}
void ReadPCIConfig()
{
	int iBus = 0;
	int iDevice = 0;
	int iTmp;
	int iData = 0;  
	char *smsg="Device Vender\r\n";
	char *sRet = "\r\n";
	DisplayMsg(smsg);
	for(;iBus<5;iBus++)
	{
		for(;iDevice<32;iDevice++)
		{
			__asm__ (
				"MOVW $0x5789, %%AX\n\t"
				:
				:
			);
			iTmp = 0x80000000+iBus*0x10000+iDevice*8*0x100;
			WritePort(0xcf8, iTmp);
			iData = ReadPort(0xcfc);
			if((iData&0xffff)!=0xffff)
			{
				DisplayWord((iData>>16)&0xffff);
				DisplayMsg("   ");
				DisplayWord(iData&0xffff);
				DisplayMsg("\r\n");
				iData = 0;
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
	char cDataHigh = (sData>>8)&0xFF;
	DisplayCharNum(cDataHigh);
	char cDataLow = (sData&0xFF);
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

void lmain(void)
{
	//char *fileName="KERNEL.BIN";
	//FDDescriptor fd;
	//GetMemConfig();
	//FindFile(fileName,&fd);
	//the return code is not right run in bochs
	//so i use jump instruction to jump back to
	//the right address(after call main)
	//DisplayMsg("\r\n");
	ReadPCIConfig();	
	/*int iRet = CmpStr("abcd","abc");
	if(0 == iRet)
	{
		DisplayMsg("equal\r\n");
	}
	else
	{
		DisplayMsg("not equal\r\n");
	}
	*/
	 __asm__ (
         	"MOVL %%EBP,%%ESP\n\t"
         	"POPL %%EBP\n\t"
		"POPW %%AX\n\t"
		"JMP %%AX\n\t"
           	:
           	:
        );
}


