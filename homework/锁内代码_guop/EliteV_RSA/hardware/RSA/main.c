#include <stdlib.h>
#include <string.h>
#include "h5ses.h"

int main(void)
{
	UINT16 ret=	 SES_SUCCESS;
	UINT8 *input=_get_input();
	UINT16 input_len=_get_input_len();
	UINT8 output[1024]={0};
	UINT16 output_len=sizeof(output);
	UINT8 *pu8SignatureBuf=output;
	UINT16 ret_len=0;
	CHAR pri_fid[]={"rsa_1024_pri.slv"};
	HANDLE hFile=0;
	
	ret=_file_open(pri_fid,&hFile);	
	if(ret!=SES_SUCCESS)
	{
		_set_output((UINT8 *)&ret,2);
		return -1;
	}
		
	ret=_rsa_sign_keyfile(RSA_PAD_MODE_PKCS,HASH_SHA1,hFile,input,input_len,pu8SignatureBuf,output_len,(UINT32 *)&ret_len);	
	if(ret!=SES_SUCCESS)
	{
		_set_output((UINT8 *)&ret,2);
		return -1;
	}
	
	_set_output((UINT8 *)pu8SignatureBuf,ret_len);
	
	return 0;
}
