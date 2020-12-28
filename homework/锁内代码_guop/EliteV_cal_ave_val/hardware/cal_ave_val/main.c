#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "h5ses.h"

void cal_val(float *input,int input_size)
{
	int i=0;
	float sum=0;
	for(i=0;i<input_size;i++)
	{
		sum+=*input;
		input++;
	}
	*input=sum/input_size;
}

int main(void)
{
	UINT16 inLen  = _get_input_len();
	float *inBuf = (float *)_get_input();
	cal_val((float *)inBuf, inLen/sizeof(float));
	_set_output((UINT8 *)(inBuf+inLen/sizeof(float)), sizeof(float));
	return 0;
}
