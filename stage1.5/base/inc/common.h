#ifndef _COMMON_H_
#define _COMMON_H_
#include "typedefs.h"
#define sizeof_t(type)  ((size_t)((type *)0 + 1))
void itoax(u32 i, char *str);
void ctoax(u8 c, char *str);
#endif