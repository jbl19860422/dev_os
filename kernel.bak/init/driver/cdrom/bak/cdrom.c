#include "cdrom.h"
DWORD g_iDriverNum = 0;
void InitCDRom()
{
	g_iDriverNum = *((int *)(0x8400-4));
	PrintMsg("DriverNum:");
	PrintDword(g_iDriverNum);
	PrintMsg("\r\n");
}

int GetFirstFileName(char *sFile, char *sFirstFile)
{
	int iPos = 0;
	while(sFile[iPos] == '/')
	{
		iPos++;
	}
	iPos++;
	
	int i = 0;
	while(sFile[iPos] != '/' && sFile[iPos] != '\0')
	{
		sFirstFile[i] = sFile[iPos];
		i++;
		iPos++;
	}
	return iPos;
}
int SearchFile(char *sFileName, FD_DESCRIPTOR *fd)
{
	char sFileNameTmp[256];
	FD_DESCRIPTOR fd_curr;
	int iRet = GetRootFd(&fd_curr);
	if(0 != iRet)
	{
		PrintMsg("GetRootFd err.\r\n");
		return -1;
	}
	
	int iNextSplashPos = GetFirstFileName(sFileName, sFileNameTmp);
	FD_DESCRIPTOR fd_next;
	while(sFileName[iNextSplashPos] != '\0')
	{
		iRet = _FindFileByFd(sFileNameTmp, &fd_curr, &fd_next);
		if(1 != iRet)
		{
			return -2;
		}
		iNextSplashPos = GetFirstFileName(&sFileName[iNextSplashPos], sFileNameTmp);
	}
	
	*fd = fd_next;
}

int GetRootFd(FD_DESCRIPTOR *root_fd)
{
	*fd = NULL;
	int rootSectorBuff = GetFreePage(1);
	if(rootSectorBuff == NULL)
	{
		return -1;
	}
	short seg, off;
	DecodeAddrTo16Mode(rootSectorBuff,&seg,&off);
	//读取根卷标
  	iRet = ReadSector(CDROM_ROOT_SECTOR,0x0,1,seg,off);
	if(0 != iRet)
	{
		return -2;
	}
	FD_DESCRIPTOR *pRootDirRec = ((VOLUME_DESCRIPTOR *)rootSectorBuff)->RootDirRec;
	*root_fd = *pRootDirRec;
	return 0;
}
/**查找一个目录中是否有想要的文件，找到了返回对应的fd_find**/
int _FindFileByFd(char *sFileName, FD_DESCRIPTOR *fd, FD_DESCRIPTOR *fd_find)
{
	int iSector = fd->iLocOfExtBig;//一般系统都是大端格式，后面再改吧
	int iSectorCount = (fd->iDataLenBig/CDROM_LEN_PER_SECTOR)+1;
	char *buffer = GetFreePage(((iSectorCount*CDROM_LEN_PER_SECTOR+4096)/4096));
	short seg,off;
	DecodeAddrTo16Mode(buffer,&seg,&off);
	//读取根目录内容
	iRet = ReadSector(iSector,0x0,iSectorCount,seg,off);
	if(0 != iRet)
	{
		return -1;
	}
	FD_DESCRIPTOR *pRec = (FD_DESCRIPTOR *)buffer;
	char cLen= pRec->cDirRecLen;
	int iFindFile = 0;
	while(cLen>0)
	{
		if(0 == CmpStr32(pRec->sFileName, sFileName))
		{
			iFindFile = 1;
			*fd_find = *pRec;
			break;
		}
		pRec = (FD_DESCRIPTOR*)((char *)pRec+cLen);
		cLen = pRec->cDirRecLen;
	}
	return iFindFile;
}
int FindRootFile(char *sFileName,FD_DESCRIPTOR* fd)
{
	//PrintMsg("start to find file\r\n");
	int iRet = 0;
	//分配内存，读取卷标
	*fd = NULL;
	int rootSectorBuff = GetFreePage(1);
	if(rootSectorBuff == NULL)
	{
		PrintMsg("GetFreePage err.");
		return -1;
	}
	short seg, off;
	DecodeAddrTo16Mode(rootSectorBuff,&seg,&off);
	//读取根卷标
  	iRet = ReadSector(CDROM_ROOT_SECTOR,0x0,1,seg,off);
	PrintMsg("afdasfsd\r\n");
	if(0 != iRet)
	{
		PrintMsg("read sector err\r\n");
		return -2;
	}
	PrintMsg("end to read file\r\n");
	FD_DESCRIPTOR *pRootDirRec = ((VOLUME_DESCRIPTOR *)rootSectorBuff)->RootDirRec;// 第16扇区存储的是一个卷描述符,其中有一个根目录的描述符
	int iRootSector = pRootDirRec->iLocOfExtBig;//一般系统都是大端格式，后面再改吧
	int iRootSectorCount = (pRootDirRec->iDataLenBig/CDROM_LEN_PER_SECTOR)+1;
	FreePage(rootSectorBuff);
	int buffer = GetFreePage(iRootSectorCount*CDROM_LEN_PER_SECTOR/4096+1);
	DecodeAddrTo16Mode(buffer,&seg,&off);
	//读取根目录内容
	iRet = ReadSector(iRootSector,0x0,iRootSectorCount,seg,off);
	if(0 != iRet)
	{
		return -3;
	}
	FD_DESCRIPTOR *pRec = (FD_DESCRIPTOR *)buffer;
	char cLen= pRec->cDirRecLen;
	int iFindFile = 0;
	while(cLen>0)
	{
		if(0 == CmpStr32(pRec->sFileName, sFileName))
		{
			iFindFile = 1;
			*fd = *pRec;
			break;
		}
		pRec = (FD_DESCRIPTOR*)((char *)pRec+cLen);
		cLen = pRec->cDirRecLen;
	}
	if(iFindFile == 1)
	{
		char *msg="find file\r\n";
		PrintMsg(msg);
	}
	else
	{
		char *msg1="not find file\r\n";
		PrintMsg(msg1);
	}
	/*
	__asm__ (
                "MOVL %%EBP,%%ESP\n\t"
                "POPL %%EBP\n\t"
                "POPW %%BX\n\t"
		"MOVW $1,%%AX\n\t"
                "JMP %%BX\n\t"
                :
                :
        );
	*/
	FreePage(buffer);
	return iFindFile;
}
