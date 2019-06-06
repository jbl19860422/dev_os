#include "vga.h"
#include "cdrom.h"
#include "ata.h"
#include "pci.h"
extern void init();
extern void common_interrupt();
extern void int_process0x21();
extern void common_ret_from_int();
extern void UserInt();
extern void MouseInt();
void lmain1_5(void)
{
	//PrintMsg("GetEBDASeg=");
	//PrintDword(GetEBDASeg());
	//DetectATA();
	PrintMsg("mouse int=");PrintDword(MouseInt);PrintMsg("\r\n");
	init();
	PrintMsg("init end\r\n");
	//PrintMsg("11\r\n");
	/*
	pci_device_t devs[MAX_CLASS_DEVICE];
	u8 class = 0x1;
	u8 subclass = 0x01;
	u32 count = 0;
	find_pci_devices_by_class(class, subclass, devs, &count);
	PrintMsg("22\r\n");
	int m = 0;
	for(m = 0; m < count; m++)
	{
		PrintMsg("baseaddr0=");PrintDword(devs[m].config_head.u32_base_addr[0]);PrintMsg("\r\n");
		PrintMsg("baseaddr1=");PrintDword(devs[m].config_head.u32_base_addr[1]);PrintMsg("\r\n");
		PrintMsg("baseaddr2=");PrintDword(devs[m].config_head.u32_base_addr[2]);PrintMsg("\r\n");
		PrintMsg("baseaddr3=");PrintDword(devs[m].config_head.u32_base_addr[3]);PrintMsg("\r\n");
		PrintMsg("baseaddr4=");PrintDword(devs[m].config_head.u32_base_addr[4]);PrintMsg("\r\n");
		PrintMsg("baseaddr5=");PrintDword(devs[m].config_head.u32_base_addr[5]);PrintMsg("\r\n");
		PrintMsg("irq=");PrintDword(devs[m].config_head.u8_irqline);PrintMsg("\r\n");
		PrintMsg("pi=");PrintDword(devs[m].config_head.u8_PIFlag);PrintMsg("\r\n");
	}
	*/
	//PrintMsg("33\r\n");
	//PrintMsg("count=");PrintDword(count);PrintMsg("\r\n");
	
	//PrintMsg("init end\r\n");
	/*classcode[0]=0x0;
	find_pci_devices_by_class(classcode, devs, &count);
	m = 0;
	for(m = 0; m < count; m++)
	{
		PrintMsg("baseaddr0=");PrintDword(devs[m].config_head.u32_base_addr[0]);PrintMsg("\r\n");
		PrintMsg("baseaddr1=");PrintDword(devs[m].config_head.u32_base_addr[1]);PrintMsg("\r\n");
		PrintMsg("baseaddr2=");PrintDword(devs[m].config_head.u32_base_addr[2]);PrintMsg("\r\n");
		PrintMsg("baseaddr3=");PrintDword(devs[m].config_head.u32_base_addr[3]);PrintMsg("\r\n");
		PrintMsg("baseaddr4=");PrintDword(devs[m].config_head.u32_base_addr[4]);PrintMsg("\r\n");
		PrintMsg("baseaddr5=");PrintDword(devs[m].config_head.u32_base_addr[5]);PrintMsg("\r\n");
	}*/
	//PrintMsg("comm=");PrintDword(int_process0x21);PrintMsg("\r\n");
	//PrintMsg("c=");PrintDword(common_ret_from_int);PrintMsg("\r\n");
	//PrintMsg("userint=");PrintDword(UserInt);PrintMsg("\r\n");
	__asm__
	(
		"STI\n\t"
		:
		:
	);
	/*
	__asm__
	(
		"INT $0x2C\n\t"
		:
		:
	);
	*/
	return;
	ReadMemConf();
	InitPageConf();
	InitCDRom();
	unsigned p[40];
	int iCount = 0;
	int iRet = GetSupportMode(p, &iCount);
	if(0 != iRet)
	{
		PrintMsg("GetSuppFail\r\n");
	}
	else
	{
		int i = 0;
		for(i = 0;i < iCount;i++)
		{
			PrintMsg("mode:");PrintDword(p[i]);PrintMsg("\r\n");
		}
	}
	
	
	iRet = CheckIfSupportMode(0x115);
	if(0 != iRet)
	{
		//PrintMsg("support\r\n");
		return;
	}
	//return;
	char *sFile = "/PICTURE/BZ.BMP";
	FD_DESCRIPTOR fd;
	iRet = SearchFile(sFile, &fd);
	if(0 == iRet)
	{
		//PrintMsg("Find bmp\r\n");
	}
	else
	{
		PrintMsg("Not Find bmp\r\n");
		return;
	}
	//return;
	
	int iFileLength = fd.iDataLenBig;
	//PrintDword(iFileLength);
	//PrintMsg("\r\n");
	//PrintMsg(fd.sFileName);
	//PrintMsg("\r\n");
	//char *buffer = GetFreePage(iFileLength/4096+1);
	char *buffer = 0x100000;
	iRet = ReadFile(&fd, buffer);
	//FreePage(buffer);
	//开始画图程序啦
	InitVGA();
	/**画背景*/
	DrawBmp888(0,0,buffer);
	/**画底部*/
	char *sFileStartBtn = "/PICTURE/STARTBTN_NORMAL.BMP";
	iRet = SearchFile(sFileStartBtn, &fd);
	if(0 != iRet)
	{
		return;
	}
	*buffer = 0x100000;
	iRet = ReadFile(&fd, buffer);
	DrawBmp888(0, 600-30, buffer);
	/*显示一个汉字*/
	char *maskbuff = NULL;
	maskbuff = 0x100000;
	char *sFileMask = "/FONTS/HZK16";
	iRet = SearchFile(sFileMask, &fd);
	if(0 != iRet)
	{
		PrintMsg("not find mask\r\n");
	}
	iRet = ReadFile(&fd, maskbuff);
	RGB rgb = {0xFF,0,0};
	char code[] = {0xce,0xd2};
	DisplayHZ(0,0,code, rgb, maskbuff);
	char *maskbuff_ascii = 0x200000;
	char *sFileMask_ascii = "/FONTS/ASC8_16";
	iRet = SearchFile(sFileMask_ascii, &fd);
	if(0 != iRet)
	{
		PrintMsg("not find mask\r\n");
	}
	iRet = ReadFile(&fd, maskbuff_ascii);
	DisplayAscii(20, 40, 'C', maskbuff_ascii, rgb);
	//return;
	/**显示一个文件中的内容**/
	int iFileLen = 0;
	unsigned char *filebuff = 0x400000;
	char *sFileTxt = "/TEXT/TEXT.TXT";
	iRet = SearchFile(sFileTxt, &fd);
	if(0 != iRet)
	{
		PrintMsg("not find mask\r\n");
	}
	
	iRet = ReadFile(&fd, filebuff);
	int y0 = 40;
	int x0 = 0;
	int i;
	for(i=0; i < fd.iDataLenBig;i++)
	{
		if(filebuff[i] < 127)
		{//ascii 字符
			DisplayAscii(x0, y0, filebuff[i], maskbuff_ascii, rgb);
			x0+=16;
		}
		else
		{
			char code1[2];
			code1[0] = filebuff[i];
			i++;
			code1[1] = filebuff[i];
			DisplayHZ(x0, y0, code1, rgb, maskbuff);
			x0+=16;
		}
	}
}
