#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h5ses.h"

void bubble_sort(  long *p, int len)
{
	int i,j;
	long tmp;
	
	for (i=0;i<len-1;i++) 
	{
		for (j=0;j<len-i-1;j++) 
		{
			if (p[j] < p[j+1]) 
			{
				tmp = p[j];
				p[j] = p[j+1];
				p[j+1] = tmp;
			}
		}
	}
}

int main(void)
{
	UINT16 inLen  = _get_input_len();
	long *inBuf = (long *)_get_input();
	bubble_sort(( long *)inBuf, inLen/sizeof(int));
	_set_output((UINT8 *)inBuf, inLen);
}
