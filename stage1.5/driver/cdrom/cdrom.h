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
#define CDROM_ROOT_SECTOR 0x10//iso9660�ĵ�16�������Ǹ�����
#define CDROM_LEN_PER_SECTOR 2048//ÿ���������ֽ���
DWORD g_iDriverNum;


void InitCDRom();
//@function: Check if the cd-rom supports int13h
//@return: 0 if support, -1--not support
int checkInt13H();

//@function:read a sector from the boot device
//@param[in] dBlockAddr0:Ҫ��ȡ�ڼ��������ĵ�λ
//@param[in] dBlockAddr1:Ҫ��ȡ��������λ
//@param[in] wBlockNum:��ȡ������������
//@param[in] wBufferSeg:��ȡ�����ݴ�ŵĶε�ַ
//@param[in] wBufferOff:��ȡ�����ݴ�ŵ�ƫ�Ƶ�ַ 
//@return ��ȡ�Ƿ�ɹ� 0--�ɹ�����0--ʧ��
int ReadSector(DWORD dBlockAddr0, DWORD dBlockAddr1,DWORD wBlockNum, DWORD wBufferSeg, DWORD wBufferOff);

int FindRootFile(char *sFileName,FD_DESCRIPTOR* fd);

int GetFirstFileName(char *sFile, char *sFirstFile);

int SearchFile(char *sFileName, FD_DESCRIPTOR *fd);

int GetRootFd(FD_DESCRIPTOR *root_fd);

int _FindFileByFd(char *sFileName, FD_DESCRIPTOR *fd, FD_DESCRIPTOR *fd_find);

int ReadFile(FD_DESCRIPTOR *fd, char *buff);

#endif




