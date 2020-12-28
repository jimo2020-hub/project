#include "ses_v3.h"
void calc_val(float *input,int input_size)
{
	int i;
	float sum=0.0;
	float val=0.0;
	for(i=0;i<input_size/sizeof(float);i++)
	{
		sum=_addf(sum,*input);
		input++;
	}
	*input=sum/(input_size/sizeof(float));

}
											

void main(void)
{
	float *input=pbInBuff;
	int len=bInLen;
	calc_val(input,len);									 
	_set_response(sizeof(float),(BYTE*)(input+len/sizeof(float)));
	_exit();
}													  
					
