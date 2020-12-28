#include <string.h>
#include <stdlib.h>
#include "ses_v3.h"
#include "sample_27.h"

#define _DEBUG

WORD pri_fid=0xef01;
BYTE Hash[255]={0};

void main(void)
{
	BYTE ret=0;
	BYTE len=bInLen;
	memcpy(Hash, pbInBuff, len);
	ret=_rsa_sign(RSA_CALC_HASH|RSA_CALC_PKCS,pri_fid,len,Hash);
	if(ret!=SES_SUCCESS)
	{
		_set_response(1,&ret);
		_exit();
	}
	_set_response(128,Hash);
	_exit();
	
}
 



