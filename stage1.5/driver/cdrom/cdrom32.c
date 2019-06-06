#include "cdrom.h"
DWORD g_iDriverNum = 0;
void InitCDRom()
{
	//g_iDriverNum = *((int *)(0x8400-4));
	//PrintMsg("DriverNum:");
	//PrintDword(g_iDriverNum);
	//PrintMsg("\r\n");
}

int GetFirstFileName(char *sFile, char *sFirstFile)
{
	int iPos = 0;
	while(sFile[iPos] == '/')
	{
		iPos++;
	}
	
	int i = 0;
	while(sFile[iPos] != '/' && sFile[iPos] != '\0')
	{
		sFirstFile[i] = sFile[iPos];
		i++;
		iPos++;
	}
	sFirstFile[i] = '\0';
	return iPos;
}
int SearchFile(char *sFileName, FD_DESCRIPTOR *fd)
{
	char sFileNameTmp[256];
	FD_DESCRIPTOR fd_curr;
	int iRet = GetRootFd(&fd_curr);
	if(0 != iRet)
	{
		PrintMsg("GetRootFd err.code=");PrintDword(iRet);PrintMsg("\r\n");
		return -1;
	}
	
	int iNextSplashPos = GetFirstFileName(sFileName, sFileNameTmp);
	
	FD_DESCRIPTOR fd_next;
	while(sFileName[0] != '\0')
	{
		sFileName = &sFileName[iNextSplashPos];
		PrintMsg("going to search file:");
		PrintMsg(sFileNameTmp);
		PrintMsg("\r\n");
		iRet = _FindFileByFd(sFileNameTmp, &fd_curr, &fd_next);
		if(1 != iRet)
		{	
			PrintMsg("Not Find...\r\n");
			return -2;
		}
		PrintMsg("Find...\r\n");
		fd_curr = fd_next;
		iNextSplashPos = GetFirstFileName(sFileName, sFileNameTmp);
	}
	
	*fd = fd_next;
	return 0;
}

int GetRootFd(FD_DESCRIPTOR *root_fd)
{
	int rootSectorBuff = GetFreePage(1);
	if(rootSectorBuff == NULL)
	{
		return -1;
	}
	short seg, off;
	DecodeAddrTo16Mode(rootSectorBuff,&seg,&off);
	//读取根卷标
  	int iRet = ReadSector(CDROM_ROOT_SECTOR,0x0,1,seg,off);
	if(0 != iRet)
	{
		PrintMsg("read sector err.code=");PrintDword(iRet);PrintMsg("\r\n");
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
	int iRet = ReadSector(iSector,0x0,iSectorCount,seg,off);
	if(0 != iRet)
	{
		FreePage(buffer);
		return -1;
	}
	FD_DESCRIPTOR *pRec = (FD_DESCRIPTOR *)buffer;
	char cLen= pRec->cDirRecLen;
	int iFindFile = 0;
	while(cLen>0)
	{
		//PrintMsg(pRec->sFileName);
		//PrintMsg("\r\n");
		char sFileNameTmp[256];
		memcpy(sFileNameTmp, pRec->sFileName, pRec->cFileIdenLen);
		sFileNameTmp[pRec->cFileIdenLen] = '\0';
		PrintMsg(sFileNameTmp);PrintMsg(" ");
		if(0 == CmpStr32(sFileNameTmp, sFileName))
		{
			iFindFile = 1;
			*fd_find = *pRec;
			break;
		}
		pRec = (FD_DESCRIPTOR*)((char *)pRec+cLen);
		cLen = pRec->cDirRecLen;
	}
	FreePage(buffer);
	return iFindFile;
}
int FindRootFile(char *sFileName,FD_DESCRIPTOR* fd)
{
	//PrintMsg("start to find file\r\n");
	int iRet = 0;
	//分配内存，读取卷标
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

int ReadFile(FD_DESCRIPTOR *fd, char *buff)
{
	short seg, off;
	int iSector = fd->iLocOfExtBig;//一般系统都是大端格式，后面再改吧
	int iSectorCount = (fd->iDataLenBig/CDROM_LEN_PER_SECTOR)+1;
	char *buffer_src = GetFreePage(CDROM_LEN_PER_SECTOR/4096+1);
	DecodeAddrTo16Mode(buffer_src,&seg,&off);
	int i = 0;
	for(i = 0;i < iSectorCount;i++)
	{
		int iRet = ReadSector(iSector+i,0x0,1,seg,off);
		if(0 != iRet)
		{
			FreePage(buffer_src);
			return -1;
		}
		memcpy(buff, buffer_src, CDROM_LEN_PER_SECTOR);
		buff+=CDROM_LEN_PER_SECTOR;
	}
	FreePage(buffer_src);
	return 0;
}