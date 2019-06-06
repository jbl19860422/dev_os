#ifndef _BMP_H_
#define _BMP_H_
#pragma pack(1)
typedef struct
{
    short type;//---------�ļ����ͣ�һ���ǡ�BM��
    int bfSize;//---------�ļ���С���ֽڵ�λ
    short re1,re2;//------����λ
    int Offbits;//--------λͼ����ƫ��������������ļ���ͷ��ƫ�������ֽڵ�λ
}BITMAPFILEHEADER;

typedef struct
{
    long size;//---------------λͼ��С����һ����Ч�ġ�
    long width,height;//-------λͼ��Ⱥ�λͼ�߶ȣ����ص�λ
    short planes,bitCount;//---ƽ������һ��Ϊ1��ɫ����ȣ�������1��4��8��16���ֱ��ʾ��ɫ��16ɫ��256ɫ��16λɫ��
    long comp,sizeImg;//-------ѹ����ʽ��0��ʾ��ѹ����1��ʾRLEѹ����2��ʾÿ������4���ص�RLEѹ����
    long xpels,ypels;//--------ˮƽ�ֱ��ʺʹ�ֱ�ֱ��ʣ�����/�� ��ʾ
    long used,important;//-----��ʵ��ʹ�õ���ɫ���е���ɫ������һ����Ч����Ҫ����ɫ����Ҳ��һ����Ч
}BITMAPINFO;

typedef struct 
{
    unsigned char blue;//--------��ɫ����
    unsigned char green;//-------��ɫ����
    unsigned char red;//---------��ɫ����
    unsigned char re;//----------����
}BITMAPCOLOR;
#pragma pack()
#endif
