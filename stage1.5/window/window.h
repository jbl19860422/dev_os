#ifndef _WINDOW_H_
#define _WINDOW_H_
#include "list.h"
typedef struct
{
	int x0,y0,x1,y1;
}rec_t;// 矩形

typedef void (f_draw_window)(rec_t rec);//窗口的绘制方法

typedef struct
{
	struct list_head head;
	rec_t rec;//窗口占用的位置信息
	f_draw_window *draw_window;
}window_t;

struct list_head g_window_head;//窗口链表头
#endif