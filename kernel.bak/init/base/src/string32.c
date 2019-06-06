#include "string.h"
int CmpStr32(char *s1, char *s2)
{
	int i = 0;
	int equal = -1;
	while(s1[i] == s2[i])
	{
		if(s1[i] == '\0' && s2[i] == '\0')
		{
			equal = 0;
			break;
		}
		else if(s1[i] == '\0' || s2[i] == '\0')
		{
			break;
		}
		i++;
	}
	return equal;
}