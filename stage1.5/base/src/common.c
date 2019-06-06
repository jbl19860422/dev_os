#include "common.h"
void itoax(u32 i, char *str)
{
	u8 tmp;
	tmp = (i&0xf0000000)>>28;
	str[0] = tmp>=10?(tmp-10+'A'):(tmp+'0');
	tmp = (i&0x0f000000)>>24;
	str[1] = tmp>=10?(tmp-10+'A'):(tmp+'0');
	tmp = (i&0x00f00000)>>20;
	str[2] = tmp>=10?(tmp-10+'A'):(tmp+'0');
	tmp = (i&0x000f0000)>>16;
	str[3] = tmp>=10?(tmp-10+'A'):(tmp+'0');
	tmp = (i&0x0000f000)>>12;
	str[4] = tmp>=10?(tmp-10+'A'):(tmp+'0');
	tmp = (i&0x00000f00)>>8;
	str[5] = tmp>=10?(tmp-10+'A'):(tmp+'0');
	tmp = (i&0x000000f0)>>4;
	str[6] = tmp>=10?(tmp-10+'A'):(tmp+'0');
	tmp = (i&0x0000000f)>>0;
	str[7] = tmp>=10?(tmp-10+'A'):(tmp+'0');
	str[8] = '\0';
}

void ctoax(u8 c, char *str)
{
	u8 tmp;
	tmp = (c&0xf0)>>4;
	str[0] = tmp>=10?(tmp-10+'A'):(tmp+'0');
	tmp = (c&0x0f)>>0;
	str[1] = tmp>=10?(tmp-10+'A'):(tmp+'0');
	str[2] = '\0';
}