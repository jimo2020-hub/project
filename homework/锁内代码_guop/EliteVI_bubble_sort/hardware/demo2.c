#include "ses_v3.h"

void bubble_sort(long *p, int len)
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

void main(void)
{
	long *test = pbInBuff;						    
	int len = bInLen;						   

	bubble_sort(test, len/sizeof(long));

	_set_response(len,(BYTE*)test); 
	_exit();
}