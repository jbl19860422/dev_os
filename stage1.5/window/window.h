#ifndef _WINDOW_H_
#define _WINDOW_H_
#include "list.h"
typedef struct
{
	int x0,y0,x1,y1;
}rec_t;// ����

typedef void (f_draw_window)(rec_t rec);//���ڵĻ��Ʒ���

typedef struct
{
	struct list_head head;
	rec_t rec;//����ռ�õ�λ����Ϣ
	f_draw_window *draw_window;
}window_t;

struct list_head g_window_head;//��������ͷ
#endif