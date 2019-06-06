#include "vga.h"
int iCurRow = 0;
int iCurCol = 0;
extern void DisplayInt(int a);
#define NULL 0
void PrintChar(int iRow, int iCol, char cColor, char cChar)
{
	WORD wTmp;
	*((char *)&wTmp) = cChar;
	*((char *)&wTmp+1) = cColor;
    vVGA[VGA_LINE_COUNT*iRow+iCol] = wTmp;
}

void PrintWord(short data)
{
	char sMsg[5];
	sMsg[4] = '\0';
	char *p = &data;
	int i;
	char tmp = (p[1]&0xf0)>>4;
	sMsg[0] = tmp>=10?(tmp+'A'-10):(tmp+'0');
	tmp = p[1]&0x0f;
	sMsg[1] = tmp>=10?(tmp+'A'-10):(tmp+'0');
	tmp = (p[0]&0xf0)>>4;
	sMsg[2] = tmp>=10?(tmp+'A'-10):(tmp+'0');
	tmp = p[0]&0x0f;
	sMsg[3] = tmp>=10?(tmp+'A'-10):(tmp+'0');
	PrintMsg(sMsg);
}

void PrintDword(int iData)
{
	PrintMsg("0x");
	PrintWord((iData>>16)&0xffff);
	PrintWord(iData&0xffff);
}
void PrintMsg(char *sMsg)
{
	int i = 0;
	while(sMsg[i] != '\0')
	{	
		if(sMsg[i] == '\r')
		{
			iCurCol = 0;
			i++;
			continue;
		}
		else if(sMsg[i] == '\n')
		{
			iCurRow++;
			i++;
			continue;
		}
		else if(sMsg[i] == '\t')
		{
			iCurCol+=4;
			if(iCurCol%VGA_LINE_COUNT == 0)
			{
				iCurCol = 0;
				iCurRow++;
			}
			i++;
			continue;
		}
		PrintChar(iCurRow, iCurCol, 0xc, sMsg[i]);
		iCurCol++;
		if(iCurCol%VGA_LINE_COUNT == 0)
		{
			iCurCol = 0;
			iCurRow++;
		}
		i++;
	}
	
	return;
}

void DrawRec(int x0, int y0, int x1, int y1, char cColor)
{
	int i = 0;
	int j = 0;
	for(j = y0; j < y1; j++)
	{
		for(i = x0; i < x1; i++)
		{
			PutPixel(i, j, cColor);
		}
	}
}

void Set_BMP_Palette_Register(int index, RGB color)      //���õ�ɫ��Ĵ������� 
{ 
    //WritePortChar(PALETTE_MASK,0xff); 
    WritePortChar(PALETTE_REGISTER_WR,index); 
    WritePortChar(PALETTE_DATA,color.r>>2); 
    WritePortChar(PALETTE_DATA,color.g>>2); 
    WritePortChar(PALETTE_DATA,color.b>>2); 
} 

int GetSupportMode(unsigned *p, int *iCount)
{
	short iRet = 0;
	struct VbeInfoBlock vbeInfo;
	PrintMsg("start to get\r\n");
	__asm__ (
		"CALL switch_protect_to_real\n\t"
		".code16\n\t"
		"MOVL %%EDI,%%EAX\n\t"
		"SHRL $4,%%EAX\n\t"
		"ANDW $0xFFFF,%%AX\n\t"
		"MOVW %%AX,%%ES\n\t"
		"ANDW $0x0F,%%DI\n\t"
		"MOVW $0x4F00,%%AX\n\t"
		"INT $0x10\n\t"
		"MOVW %%AX,%%BX\n\t"
		"PUSHW %%CS\n\t"
		"CALL switch_real_to_protect\n\t"
		".code32\n\t"
		"MOVW %%BX,%%AX\n\t"
		:"=a" (iRet)
		:"D" (&vbeInfo)
	);
	PrintMsg("end get1\r\n");
	
	unsigned *m = vbeInfo.VideoModePtr;
	*iCount = 0;
	PrintMsg("m=");PrintDword(m);PrintMsg("\r\n");
	while(*m!=(unsigned)-1)
	{
		PrintDword(*m);
		p[*iCount] = *m;
		(*iCount)++;
		m++;
	}
	if(iRet == 0x004F)
	{
		return 0;
	}
	return -1;
}
char *g_pVideoBuff = NULL;
int CheckIfSupportMode(short mode)
{
	struct ModeInfo mode_info;
	short iRet = -1;
	__asm__ (
		"CALL switch_protect_to_real\n\t"
		".code16\n\t"
		"MOVL %%EDI,%%EAX\n\t"
		"SHRL $4,%%EAX\n\t"
		"ANDW $0xFFFF,%%AX\n\t"
		"MOVW %%AX,%%ES\n\t"
		"ANDW $0x0F,%%DI\n\t"
		"MOVW $0x4F01,%%AX\n\t"
		"INT $0x10\n\t"
		"MOVW %%AX,%%BX\n\t"
		"PUSHW %%CS\n\t"
		"CALL switch_real_to_protect\n\t"
		"MOVW %%BX,%%AX\n\t"
		".code32\n\t"
		"NOP"
		:"=a" (iRet)
		:"D" (&mode_info), "c" (mode)
	);
	if(iRet == 0x004F)
	{
		//PrintMsg("modeattr=");PrintDword(mode_info.ModeAttr);PrintMsg("\r\n");
		//PrintMsg("memorymodel=");PrintDword(mode_info.MemoryModel);PrintMsg("\r\n");
		//PrintMsg("XRes=");PrintDword(mode_info.XRes);PrintMsg("&YRes=");PrintDword(mode_info.YRes);PrintMsg("&BPP=");PrintDword(mode_info.BitsPerPixel);PrintMsg("\r\n");
		g_pVideoBuff = mode_info.PhyBasePtr;
		//PrintMsg("videobuff=");PrintDword(g_pVideoBuff);PrintMsg("\r\n");
		return 0;
	}
	return -1;
}

void InitBmpPalette(BITMAPCOLOR *pColor, int iCount)
{
	int i = 0;
	RGB rgb;
	for(i = 0; i < iCount; i++)
	{
		rgb.r = pColor->red;
		rgb.g = pColor->green;
		rgb.b = pColor->blue;
		Set_BMP_Palette_Register(i, rgb);
		pColor++;
	}
}

void _DrawBmp(int x0, int y0, char *pBmpData, int iWidth, int iHeight)
{
	int i = 0;
	int j = 0;
	iWidth = ((iWidth+3)/4)*4;//ÿ�ж�4�ֽڶ���
	for(j = iHeight-1; j >= 0; j--)
	{
		for(i = 0; i < iWidth; i++)
		{
			PutPixel(x0+i, y0+j, pBmpData[(iHeight-1-j)*iWidth+i]);	
		}
		//ÿ�н�������λ�� 
	}
}

void PutPixel(int x, int y, char cColor)
{
	g_pVideoBuff[y*800+x] = cColor;
}

static inline void PutPixel888(int x, int y, char r, char g, char b)
{
	int pointMemPos = 2400*y+3*x;
	g_pVideoBuff[pointMemPos] = b;
	g_pVideoBuff[pointMemPos+1] = g;
	g_pVideoBuff[pointMemPos+2] = r;
}
void InitVGA()
{
	unsigned int old_mode = get_SVGA_mode();
	//PrintMsg("old_mode=");PrintDword(old_mode);PrintMsg("\r\n");
	set_SVGA_mode(0x4118); /*Ӳ���޹��Գ�ʼ����ĻΪ640*480 256ɫģʽ*/
}
unsigned int get_SVGA_mode()
{
	unsigned int iRet = 0;
	__asm__ __volatile__(
		"CALL switch_protect_to_real\n\t"
		".code16\n\t"
		"MOVW $0x4F03,%%AX\n\t"
		"INT $0x10\n\t"
		"PUSHW %%CS\n\t"
		"CALL switch_real_to_protect\n\t"
		".code32\n\t"
		"NOP"
		:"=b" (iRet)
		:
	);
	return iRet;
}

unsigned char set_SVGA_mode(int vmode) /*����SVGA��Ļģʽ*/
{
	unsigned int iRet = 0;
	__asm__ __volatile__ (
		"CALL switch_protect_to_real\n\t"
		".code16\n\t"
		"MOVW $0x4F02,%%AX\n\t"
		"INT $0x10\n\t"
		"PUSHW %%CS\n\t"
		"CALL switch_real_to_protect\n\t"
		".code32\n\t"
		"NOP"
		:"=a" (iRet)
		: "b" (vmode)
	);
	return (iRet&0x0000ff00)>>8;
}

void DrawBmp256(int x0, int y0, char *pBmpData)
{
	int iColorCount;
	int iDataCount = 0;
	char *pBmpDataBak = pBmpData;
	BITMAPFILEHEADER *pHeader = (BITMAPFILEHEADER *)pBmpData;
	pBmpData += sizeof_t(BITMAPFILEHEADER);
	BITMAPINFO *pInfo = (BITMAPINFO *)pBmpData;
	//PrintMsg("pInfo=");PrintDword((int)pInfo);PrintMsg("\r\n");
	//PrintDword(pInfo->width);PrintMsg("\r\n");
	//PrintDword(pInfo->height);
	pBmpData+=sizeof(BITMAPINFO);
	BITMAPCOLOR *pColor = pBmpData;
	iColorCount = (pHeader->Offbits-sizeof_t(BITMAPFILEHEADER)-sizeof_t(BITMAPINFO))/sizeof_t(BITMAPCOLOR);
	//PrintMsg("color count=");PrintDword(iColorCount);PrintMsg("\r\n");
	InitBmpPalette(pColor, iColorCount);
	pBmpData =pBmpDataBak+pHeader->Offbits;
	_DrawBmp(x0, y0, pBmpData, pInfo->width, pInfo->height);
	//InitPalette();//��ԭԭ�е�ɫ�ʱ�
}

void _DrawBmp888(int x0, int y0, char *pBmpData, int iWidth, int iHeight)
{
	int i = 0;
	int j = 0;
	int nWidthBytes = iWidth*3;
	while((nWidthBytes&3)!=0)   
		nWidthBytes++;
	for(j = 0; j < iHeight-1; j++)
	{
		char *pLinesData = &pBmpData[j*nWidthBytes];
		for(i = 0; i < iWidth; i++)
		{
			char *pPointPos = &pLinesData[i*3];
			PutPixel888(x0+i, y0+(iHeight-1-j), pPointPos[2], pPointPos[1], pPointPos[0]);
		}
	}
}

void _DrawMouseBmp888(int x0, int y0, char *pBmpData, int iWidth, int iHeight)
{
	int i = 0;
	int j = 0;
	int nWidthBytes = iWidth*3;
	while((nWidthBytes&3)!=0)   
		nWidthBytes++;
	for(j = 0; j < iHeight-1; j++)
	{
		char *pLinesData = &pBmpData[j*nWidthBytes];
		for(i = 0; i < iWidth; i++)
		{
			unsigned char *pPointPos = &pLinesData[i*3];
			if(pPointPos[2] == 0xFF && pPointPos[1] == 0x00 && pPointPos[0] == 0x00)
			{//���Ǻ�ɫ�Ż�
				continue;
			}
			PutPixel888(x0+i, y0+(iHeight-1-j), pPointPos[2], pPointPos[1], pPointPos[0]);
		}
	}
}

int max(int a, int b)
{
	return a>b?a:b;
}

int min(int a, int b)
{
	return a>b?b:a;
}


void DrawRec888(int x0, int y0, int x1, int y1, RGB color)
{
	int bigX = max(x0,x1);
	int bigY = max(y0,y1);
	int smlX = min(x0,x1);
	int smlY = min(y0,y1);
	int x = smlX;
	int y = smlY;
	for(x = smlX;x < bigX;x++)
	{
		for(y = smlY;y < bigY; y++)
		{
			PutPixel888(x, y, color.r, color.g, color.b);
		}
	}
}
void DrawLine(int x0, int y0, int x1, int y1, RGB color)
{
	int bigX = max(x0,x1);
	int bigY = max(y0,y1);
	int smlX = min(x0,x1);
	int smlY = min(y0,y1);
	int x = smlX;
	int y = smlY;
	for(;x < bigX;x++)
	{
		for(;y < bigY; y++)
		{
			PutPixel888(x, y, color.r, color.g, color.b);
		}
	}
}

int DrawBmp888(int x0, int y0, char *pBmpData)
{
	int iColorCount;
	int iDataCount = 0;
	char *pBmpDataBak = pBmpData;
	BITMAPFILEHEADER *pHeader = (BITMAPFILEHEADER *)pBmpData;
	pBmpData += sizeof_t(BITMAPFILEHEADER);
	BITMAPINFO *pInfo = (BITMAPINFO *)pBmpData;
	pBmpData = pBmpDataBak + pHeader->Offbits;
	_DrawBmp888(x0, y0, pBmpData, pInfo->width, pInfo->height);
}

int DrawMouseBmp888(int x0, int y0, char *pBmpData)
{
	int iColorCount;
	int iDataCount = 0;
	char *pBmpDataBak = pBmpData;
	BITMAPFILEHEADER *pHeader = (BITMAPFILEHEADER *)pBmpData;
	pBmpData += sizeof_t(BITMAPFILEHEADER);
	BITMAPINFO *pInfo = (BITMAPINFO *)pBmpData;
	pBmpData = pBmpDataBak + pHeader->Offbits;
	_DrawMouseBmp888(x0, y0, pBmpData, pInfo->width, pInfo->height);
}

void GetHZMask(char code[2], char *mask, char *maskbuff)
{
	unsigned char qh,wh;
    unsigned long offset;
    qh=code[0]-0xa1; /*�õ�����*/
    wh=code[1]-0xa1; /*�õ�λ��*/
    offset=(94*qh+wh)*32L; /*�õ�ƫ��λ��*/
	memcpy(mask, &maskbuff[offset], 32);
}
void DisplayHZ(int x0, int y0, char code[2], RGB color, char *maskbuff)
{
	unsigned char mask[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    /*������ģÿ�и�λ������*/
    int i,j,x,y,pos;
    char mat[32];
    GetHZMask(code, mat, maskbuff);
    y=y0;
    for (i=0;i<16;++i)
    {
        x=x0;
        pos=2*i;
        for(j=0;j<16;++j)
        {
            /*���γ�������ģ��һ��λ����һ������1 ����ʾ��������ʾ�õ�*/
            if (mask[j%8] & mat[pos+j/8])
                PutPixel888(x,y,color.r, color.g, color.b);
            ++x;
        }
        ++y;
    }
}

void DisplayAscii(int x0, int y0, char c, char* maskbuff, RGB color)
{
	int i = 0,j;
	char m;
	for(i=0; i< 16; i++)//����
	{
		m = maskbuff[c*16+i];
		for(j=7;j>=0;j--)//����
		{
			if((m>>j)&0x1)//���ƶ�jλ���õ���λ��0,1����ʾ������)
			{
				PutPixel888(x0+7-j,y0+i, color.r, color.g, color.b);
			}
		}
	}
}

void DisplayStr(int x0, int y0, char *str, char* maskbuff, RGB color)
{
	int i = 0;
	int x = x0;
	while(str[i] != '\0')
	{
		if(x<600-16)
		{
			DisplayAscii(x,y0,str[i], maskbuff,color);
			x+=16;
		}
		i++;
	}
}

