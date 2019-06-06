#ifndef _CDROM_H_
#define _CDROM_H_
/***cd-rom uses iso9660 protocol***/
//#include "typedefs.h"
#include "vga.h"
#include "memory.h"
#pragma pack(1)
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
}VOLUME_DESCRIPTOR;

typedef struct
{
        BYTE cDirRecLen;
        BYTE cExtAttrRecLen;
        DWORD iLocOfExtBig;
        DWORD iLocOfExtSmall;
        DWORD iDataLenBig;
        DWORD iDataLenSmall;
        BYTE sRecDateTime[7];
        BYTE cFileFlags;
        BYTE cFileUnitSize;
        BYTE cGapSize;
        DWORD iVolSeqNum;
        BYTE cFileIdenLen;
        BYTE sFileName[256];
}FD_DESCRIPTOR;

typedef struct
{
        BYTE cPacketSize;
        BYTE cReserved;
        WORD wBlockCount;
        WORD wBufferOff;
        WORD wBufferSeg;
        DWORD dBlockAddr0;
        DWORD dBlockAddr1;
}DISK_ADDRESS_PACKET;

typedef struct file_descriptor
{
	FD_DESCRIPTOR fd;
};
#pragma pack()
#define DISK_PACKET_SIZE 0x10 //disk packet size is always 0x10
#define CDROM_ROOT_SECTOR 0x10//iso9660的第16个扇区是根扇区
#define CDROM_LEN_PER_SECTOR 2048//每个扇区的字节数
DWORD g_iDriverNum;


void InitCDRom();
//@function: Check if the cd-rom supports int13h
//@return: 0 if support, -1--not support
int checkInt13H();

//@function:read a sector from the boot device
//@param[in] dBlockAddr0:要读取第几个扇区的地位
//@param[in] dBlockAddr1:要读取的扇区高位
//@param[in] wBlockNum:读取的扇区的数量
//@param[in] wBufferSeg:读取的内容存放的段地址
//@param[in] wBufferOff:读取的内容存放的偏移地址 
//@return 读取是否成功 0--成功，非0--失败
int ReadSector(DWORD dBlockAddr0, DWORD dBlockAddr1,DWORD wBlockNum, DWORD wBufferSeg, DWORD wBufferOff);

int FindRootFile(char *sFileName,FD_DESCRIPTOR* fd);

int GetFirstFileName(char *sFile, char *sFirstFile);

int SearchFile(char *sFileName, FD_DESCRIPTOR *fd);

int GetRootFd(FD_DESCRIPTOR *root_fd);

int _FindFileByFd(char *sFileName, FD_DESCRIPTOR *fd, FD_DESCRIPTOR *fd_find);

int ReadFile(FD_DESCRIPTOR *fd, char *buff);

#endif




