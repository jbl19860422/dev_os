#ifndef _VGA_H_
#define _VGA_H_
#include "typedefs.h"
#include "common.h"
#include "bmp.h"
#include "io.h"
int iCurRow;
int iCurCol;
char cColor;
WORD vVGA[0x4000];
#define VGA_LINE_COUNT 80

#define PALETTE_MASK            0x3c6   //调色板屏蔽寄存器端口，放入0xff可以通过调色板索引寄存器0x3c7和0x3c8访问你希望的寄存器 
#define PALETTE_REGISTER_RD     0x3c7   //读颜色寄存器端口 
#define PALETTE_REGISTER_WR     0x3c8   //写颜色寄存器端口 
#define PALETTE_DATA            0x3c9   //调色板数据寄存器端口 


#define VBE_VESA_MODE_640X400X8                          0x100
#define VBE_VESA_MODE_640X480X8                          0x101
#define VBE_VESA_MODE_800X600X4                          0x102
#define VBE_VESA_MODE_800X600X8                          0x103
#define VBE_VESA_MODE_1024X768X4                         0x104
#define VBE_VESA_MODE_1024X768X8                         0x105
#define VBE_VESA_MODE_1280X1024X4                        0x106
#define VBE_VESA_MODE_1280X1024X8                        0x107
#define VBE_VESA_MODE_320X200X1555                       0x10D
#define VBE_VESA_MODE_320X200X565                        0x10E
#define VBE_VESA_MODE_320X200X888                        0x10F
#define VBE_VESA_MODE_640X480X1555                       0x110
#define VBE_VESA_MODE_640X480X565                        0x111
#define VBE_VESA_MODE_640X480X888                        0x112
#define VBE_VESA_MODE_800X600X1555                       0x113
#define VBE_VESA_MODE_800X600X565                        0x114
#define VBE_VESA_MODE_800X600X888                        0x115
#define VBE_VESA_MODE_1024X768X1555                      0x116
#define VBE_VESA_MODE_1024X768X565                       0x117
#define VBE_VESA_MODE_1024X768X888                       0x118
#define VBE_VESA_MODE_1280X1024X1555                     0x119
#define VBE_VESA_MODE_1280X1024X565                      0x11A
#define VBE_VESA_MODE_1280X1024X888                      0x11B
#define VBE_VESA_MODE_1600X1200X8                        0x11C
#define VBE_VESA_MODE_1600X1200X1555                     0x11D
#define VBE_VESA_MODE_1600X1200X565                      0x11E
#define VBE_VESA_MODE_1600X1200X888                      0x11F


#pragma pack(1)
typedef struct 
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}RGB;

char *g_pVideoBuff;
typedef struct ModeInfo //共256字节
{
    WORD ModeAttr;           //模式的属性
	BYTE WinAAttr,WinBAttr; //窗口A,B的属性    
	/*
    还有其他的位面-窗口映射方法中包含两个窗口,不过现在这种情况极少
    */
	WORD WinGran;             //位面大小(窗口粒度),以KB为单位
    WORD WinSize;             //窗口大小,以KB为单位
    WORD WinASeg, WinBSeg;    //窗口A,B的起始段址
	BYTE *BankFunc;       //换页调用入口指针
	/*
　　换页时可以调用该功能，也可以用VBE功能05h完成，但是直接调用该功能可以加快调用速度，因为int指令需要耗费大量的CPU周期。高性能的程序设计都是以直接调用该功能代替05h功能进行换页。(PS:我们暂时不编高性能的程序，所以使用int 10,AX=5F05h换页)
　　*/
    WORD BytesPerScanLine;    //每条水平扫描线所占的字节数
    WORD XRes, YRes;          //水平,垂直方向的分辨率
    BYTE XCharSize, YCharSize;//字符的宽度和高度
    BYTE NumberOfplanes;      //位平面的个数
    BYTE BitsPerPixel;        //每像素的位数
    BYTE NumberOfBanks;        //CGA逻辑扫描线分组数
    BYTE MemoryModel;         //显示内存模式
    BYTE BankSize;            //CGA每组扫描线的大小
    BYTE NumberOfImagePages;  //可同时载入的最大满屏图像数
    BYTE reserve1;            //为页面功能保留

    //对直接写颜色模式的定义区域
    BYTE RedMaskSize;          //红色所占的位数
    BYTE RedFieldPosition;     //红色的最低有效位位置
    BYTE GreenMaskSize;        //绿色所占位数
    BYTE GreenFieldPosition;   //绿色的最低有效位位置
    BYTE BlueMaskSize;         //蓝色所占位数
    BYTE BlueFieldPosition;    //蓝色最低有效位位置
    BYTE RsvMaskSize;          //保留色所占位数
    BYTE RsvFieldPosition;     //保留色的最低有效位位置
    BYTE DirectColorModeInfo;  //直接颜色模式属性

    //以下为VBE2.0版本以上定义
    BYTE *PhyBasePtr;      //可使用的大的帧缓存时为指向其首址的32位物理地址
    DWORD OffScreenMenOffset;  //帧缓存首址的32位偏移量
    WORD OffScreenMemSize;     //可用的,连续的显示缓冲区,以KB为单位

    //以下为VBE3.0版以上定义
    WORD LinBytesPerScanLine;   //线形缓冲区中每条扫描线的长度,以字节为单位
    BYTE BnkNumberOfImagePages; //使用窗口功能时的显示页面数
    BYTE LinNumberOfImagePages; //使用大的线性缓冲区时的显示页面数
    BYTE LinRedMaskSize;        //使用大的线性缓冲区时红色所占位数
    BYTE LinRedFieldPosition;   //使用大的线性缓冲区时红色最低有效位位置
    BYTE LinGreenMaskSize;      //使用大的线性缓冲区时绿色所占的位数
    BYTE LinGreenFieldPosition; //使用大的线性缓冲区时绿色最低有效位位置
    BYTE LinBlueMaskSize;       //使用大的线性缓冲区时蓝色所占的位数
    BYTE LinBlueFieldPosition;  //使用大的线性缓冲区时蓝色最低有效位位置
    BYTE LinRsvMaskSize;        //使用大的线性缓冲区时保留色所占位数
    BYTE LinRsvFieldPosition;   //使用大的线性缓冲区时保留色最低有效位位置
    BYTE reserve2[194];         //保留
};

typedef struct VbeInfoBlock{     
	char    VESASignature[4];       /* 'VESA' 4 byte signature          */     
	short   VESAVersion;            /* VBE version number               */     
	char    *OEMStringPtr;      /* Pointer to OEM string            */     
	long    Capabilities;           /* Capabilities of video card       */     
	unsigned *VideoModePtr;     /* Pointer to supported modes       */     
	short   TotalMemory;            /* Number of 64kb memory blocks     */     
	char    reserved[236];          /* Pad to 256 byte block size       */ 
};

#pragma pack()

void PrintChar(int iRow, int iCol, char cColor, char cChar);
void PrintMsg(char *sMsg);
void PrintWord(short data);
void PrintDword(int iData);

void PrintChar16(int iRow, int iCol, char cColor, char cChar);
void PrintMsg16(char *sMsg);

void PutPixel(int x0, int y0, char cColor);
void DrawRec(int x0, int y0, int x1, int y1, char cColor);
void DrawBmp256(int x0, int y0, char *pBmpData);
int DrawBmp888(int x0, int y0, char *pBmpData);
void _DrawBmp(int x0, int y0, char *pBmpData, int iWith, int iHeight);
int DrawBmpMouse888(int x0, int y0, char *pBmpData);
void _DrawMouseBmp888(int x0, int y0, char *pBmpData, int iWith, int iHeight);
void InitBmpPalette(BITMAPCOLOR *pColor, int iCount);
void Set_BMP_Palette_Register(int index, RGB color);
void InitPalette();

void InitVGA();

void DrawLine(int x0, int y0, int x1, int y1, RGB color);
void DrawRec888(int x0, int y0, int x1, int y1, RGB color);

void GetHZMask(char code[2], char *mask, char *maskbuff);
void DisplayHZ(int x0, int y0, char code[2], RGB color, char *maskbuff);
void DisplayAscii(int x0, int y0, char c, char *maskbuff, RGB color);
void DisplayStr(int x0, int y0, char *str, char* maskbuff, RGB color);

int GetSupportMode(unsigned *p, int *iCount);

int CheckIfSupportMode(short mode);

unsigned int get_SVGA_mode();

unsigned char set_SVGA_mode(int vmode); /*设置SVGA屏幕模式*/

#define COL_FFFFFF 0 
#define COL_FFFFCC 1 
#define COL_FFFF99 2 
#define COL_FFFF66 3 
#define COL_FFFF33 4 
#define COL_FFFF00 5 
#define COL_FFCCFF 6 
#define COL_FFCCCC 7 
#define COL_FFCC99 8 
#define COL_FFCC66 9 
#define COL_FFCC33 10 
#define COL_FFCC00 11 
#define COL_FF99FF 12 
#define COL_FF99CC 13 
#define COL_FF9999 14 
#define COL_FF9966 15 
#define COL_FF9933 16 
#define COL_FF9900 17 
#define COL_FF66FF 18 
#define COL_FF66CC 19 
#define COL_FF6699 20 
#define COL_FF6666 21 
#define COL_FF6633 22 
#define COL_FF6600 23 
#define COL_FF33FF 24 
#define COL_FF33CC 25 
#define COL_FF3399 26 
#define COL_FF3366 27 
#define COL_FF3333 28 
#define COL_FF3300 29 
#define COL_FF00FF 30 
#define COL_FF00CC 31 
#define COL_FF0099 32 
#define COL_FF0066 33 
#define COL_FF0033 34 
#define COL_FF0000 35 
#define COL_66FFFF 36 
#define COL_66FFCC 37 
#define COL_66FF99 38 
#define COL_66FF66 39 
#define COL_66FF33 40 
#define COL_66FF00 41 
#define COL_66CCFF 42 
#define COL_66CCCC 43 
#define COL_66CC99 44 
#define COL_66CC66 45 
#define COL_66CC33 46 
#define COL_66CC00 47 
#define COL_6699FF 48 
#define COL_6699CC 49 
#define COL_669999 50 
#define COL_669966 51 
#define COL_669933 52 
#define COL_669900 53 
#define COL_6666FF 54 
#define COL_6666CC 55 
#define COL_666699 56 
#define COL_666666 57 
#define COL_666633 58 
#define COL_666600 59 
#define COL_6633FF 60 
#define COL_6633CC 61 
#define COL_663399 62 
#define COL_663366 63 
#define COL_663333 64 
#define COL_663300 65 
#define COL_6600FF 66 
#define COL_6600CC 67 
#define COL_660099 68 
#define COL_660066 69 
#define COL_660033 70 
#define COL_660000 71 
#define COL_CCFFFF 72 
#define COL_CCFFCC 73 
#define COL_CCFF99 74 
#define COL_CCFF66 75 
#define COL_CCFF33 76 
#define COL_CCFF00 77 
#define COL_CCCCFF 78 
#define COL_CCCCCC 79 
#define COL_CCCC99 80 
#define COL_CCCC66 81 
#define COL_CCCC33 82 
#define COL_CCCC00 83 
#define COL_CC99FF 84 
#define COL_CC99CC 85 
#define COL_CC9999 86 
#define COL_CC9966 87 
#define COL_CC9933 88 
#define COL_CC9900 89 
#define COL_CC66FF 90 
#define COL_CC66CC 91 
#define COL_CC6699 92 
#define COL_CC6666 93 
#define COL_CC6633 94 
#define COL_CC6600 95 
#define COL_CC33FF 96 
#define COL_CC33CC 97 
#define COL_CC3399 98 
#define COL_CC3366 99 
#define COL_CC3333 100 
#define COL_CC3300 101 
#define COL_CC00FF 102 
#define COL_CC00CC 103 
#define COL_CC0099 104 
#define COL_CC0066 105 
#define COL_CC0033 106 
#define COL_CC0000 107 
#define COL_33FFFF 108 
#define COL_33FFCC 109 
#define COL_33FF99 110 
#define COL_33FF66 111 
#define COL_33FF33 112 
#define COL_33FF00 113 
#define COL_33CCFF 114 
#define COL_33CCCC 115 
#define COL_33CC99 116 
#define COL_33CC66 117 
#define COL_33CC33 118 
#define COL_33CC00 119 
#define COL_3399FF 120 
#define COL_3399CC 121 
#define COL_339999 122 
#define COL_339966 123 
#define COL_339933 124 
#define COL_339900 125 
#define COL_3366FF 126 
#define COL_3366CC 127 
#define COL_336699 128 
#define COL_336666 129 
#define COL_336633 130 
#define COL_336600 131 
#define COL_3333FF 132 
#define COL_3333CC 133 
#define COL_333399 134 
#define COL_333366 135 
#define COL_333333 136 
#define COL_333300 137 
#define COL_3300FF 138 
#define COL_3300CC 139 
#define COL_330099 140 
#define COL_330066 141 
#define COL_330033 142 
#define COL_330000 143 
#define COL_99FFFF 144 
#define COL_99FFCC 145 
#define COL_99FF99 146 
#define COL_99FF66 147 
#define COL_99FF33 148 
#define COL_99FF00 149 
#define COL_99CCFF 150 
#define COL_99CCCC 151 
#define COL_99CC99 152 
#define COL_99CC66 153 
#define COL_99CC33 154 
#define COL_99CC00 155 
#define COL_9999FF 156 
#define COL_9999CC 157 
#define COL_999999 158 
#define COL_999966 159 
#define COL_999933 160 
#define COL_999900 161 
#define COL_9966FF 162 
#define COL_9966CC 163 
#define COL_996699 164 
#define COL_996666 165 
#define COL_996633 166 
#define COL_996600 167 
#define COL_9933FF 168 
#define COL_9933CC 169 
#define COL_993399 170 
#define COL_993366 171 
#define COL_993333 172 
#define COL_993300 173 
#define COL_9900FF 174 
#define COL_9900CC 175 
#define COL_990099 176 
#define COL_990066 177 
#define COL_990033 178 
#define COL_990000 179 
#define COL_00FFFF 180 
#define COL_00FFCC 181 
#define COL_00FF99 182 
#define COL_00FF66 183 
#define COL_00FF33 184 
#define COL_00FF00 185 
#define COL_00CCFF 186 
#define COL_00CCCC 187 
#define COL_00CC99 188 
#define COL_00CC66 189 
#define COL_00CC33 190 
#define COL_00CC00 191 
#define COL_0099FF 192 
#define COL_0099CC 193 
#define COL_009999 194 
#define COL_009966 195 
#define COL_009933 196 
#define COL_009900 197 
#define COL_0066FF 198 
#define COL_0066CC 199 
#define COL_006699 200 
#define COL_006666 201 
#define COL_006633 202 
#define COL_006600 203 
#define COL_0033FF 204 
#define COL_0033CC 205 
#define COL_003399 206 
#define COL_003366 207 
#define COL_003333 208 
#define COL_003300 209 
#define COL_0000FF 210 
#define COL_0000CC 211 
#define COL_000099 212 
#define COL_000066 213 
#define COL_000033 214 
#define COL_000000 215 
#endif
