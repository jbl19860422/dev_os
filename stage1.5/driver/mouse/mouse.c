#include "mouse.h"
u8 *pMouseBmpData = NULL;
int InitMouseBmp(char *sBmpFile)
{
	pMouseBmpData = 0x600000;
	FD_DESCRIPTOR fd;
	int iRet = SearchFile(sBmpFile, &fd);
	if(0 != iRet)
	{
		return -1;
	}
	iRet = ReadFile(&fd, pMouseBmpData);
	if(0 != iRet)
	{
		return -2;
	}
	return 0;
}

void DrawMouse(rec_t rec)
{
	DrawMouseBmp888(rec.x0, rec.y0, pMouseBmpData);
}