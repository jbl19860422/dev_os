#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "vga.h"
#include "cdrom.h"
#include "window.h"
typedef struct
{
	u8 flag;
	u8 deltx;
	u8 delty;
}mouse_data_t;
window_t mouse_window;
u8 *pMouseBmpData;
int InitMouseBmp(char * sBmpFile);
void DrawMouse(rec_t rec);
#endif