#ifndef _BMP_H_
#define _BMP_H_
#pragma pack(1)
typedef struct
{
    short type;//---------文件类型，一定是‘BM’
    int bfSize;//---------文件大小，字节单位
    short re1,re2;//------保留位
    int Offbits;//--------位图矩阵偏移量，是相对于文件开头的偏移量，字节单位
}BITMAPFILEHEADER;

typedef struct
{
    long size;//---------------位图大小，不一定有效的。
    long width,height;//-------位图宽度和位图高度，象素单位
    short planes,bitCount;//---平面数，一定为1；色彩深度，可以是1，4，8，16，分别表示单色，16色，256色和16位色。
    long comp,sizeImg;//-------压缩方式，0表示无压缩，1表示RLE压缩，2表示每个象素4比特的RLE压缩。
    long xpels,ypels;//--------水平分辨率和垂直分辨率，象素/米 表示
    long used,important;//-----所实际使用的颜色表中的颜色数，不一定有效；重要的颜色数，也不一定有效
}BITMAPINFO;

typedef struct 
{
    unsigned char blue;//--------蓝色亮度
    unsigned char green;//-------绿色亮度
    unsigned char red;//---------红色亮度
    unsigned char re;//----------保留
}BITMAPCOLOR;
#pragma pack()
#endif
