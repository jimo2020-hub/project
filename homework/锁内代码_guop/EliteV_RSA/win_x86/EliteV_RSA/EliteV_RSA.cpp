#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>
#include "ss_lm_runtime.h"
#include "ss_define.h"
#include "slmCrypto.h"
#define MAX_BUFFER_SIZE 1024

unsigned char pub_key[152]={  
	0x01, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x00, 0x04, 0x8C, 0x00, 0x30, 0x81, 0x89, 0x02,
	0x81, 0x81, 0x00, 0xB3, 0x95, 0xBB, 0x16, 0xA6, 0x43, 0x71, 0x13, 0x47, 0xD6, 0xAE, 0x39, 0x53,
	0xD2, 0x46, 0xC4, 0xF0, 0xFD, 0x9C, 0xAA, 0x54, 0x65, 0x4B, 0x26, 0x54, 0xCE, 0xDA, 0xDF, 0x4A,
	0x70, 0xB0, 0x97, 0x7B, 0x61, 0x0E, 0xD2, 0x3B, 0x43, 0xE4, 0x99, 0xC5, 0x96, 0x2D, 0x23, 0x8D,
	0x05, 0xEF, 0xFD, 0x17, 0x6F, 0x30, 0x04, 0xBB, 0x8B, 0xC9, 0x0B, 0x2C, 0xA9, 0x5D, 0x4B, 0xB7,
	0xB7, 0x59, 0x12, 0xF0, 0x92, 0x42, 0x80, 0x6D, 0x52, 0xA0, 0x1F, 0xF6, 0x26, 0x4B, 0x91, 0xE0,
	0x56, 0xB3, 0x6A, 0x69, 0xE4, 0xCA, 0xC0, 0x1B, 0x1E, 0x9D, 0x50, 0x31, 0x4F, 0x6F, 0xAC, 0x3F,
	0x06, 0xAE, 0xC0, 0x96, 0xC8, 0x27, 0x24, 0x8E, 0xF6, 0xC2, 0xF5, 0xFA, 0xE9, 0xC4, 0xFA, 0x15,
	0xA5, 0xFF, 0x73, 0xFC, 0x6C, 0x7C, 0x5C, 0xD7, 0xE4, 0x47, 0x92, 0x92, 0xB6, 0x75, 0x64, 0x8E,
	0x49, 0x4A, 0xB3, 0x02, 0x03, 0x01, 0x00, 0x01
	}; 
int main(int argc, char* argv[])
{
	srand(time(NULL));
	unsigned long length = rand() % (129 - 16) + 16;
	unsigned char *lpToBeSigned = (unsigned char*)malloc(length + 1);
	int i = 0;
	if (lpToBeSigned == NULL)
	{
		perror("malloc error:\n");
		return -1;
	}
	for (i = 0; i < length; i++)
	{
		*(lpToBeSigned + i) = (unsigned char)(rand() % 0xFF);
	}
	printf("lpToBeSigned is:\n");
	for (i = 0; i < length; i++)
	{
		printf("%02X", *(lpToBeSigned + i));
	}
	printf("\n");
	SS_UINT32 ret = 0;
	ST_INIT_PARAM init_param = { 0 };
	ST_LOGIN_PARAM login_param = { 0 };
	SLM_HANDLE_INDEX slm_handle = 0;
	SS_UINT32 ret_len = 0;
	init_param.version = SLM_CALLBACK_VERSION02;
	init_param.pfn = NULL;
	init_param.timeout = 0;
	SS_BYTE output[MAX_BUFFER_SIZE] = { 0 };
	unsigned char *pbSignature[1024] = { 0 };
	unsigned char g_api_password[16] = { 0xD7,0xAA,0x1D,0xEE,0x34,0x95,0xFA,0xA0,0xE8,0x11,0x61,0xFA,0xBB,0x75,0x26,0x87 };
	memcpy(init_param.password, g_api_password, sizeof(g_api_password));

	//初始化
	ret = slm_init(&(init_param));
	if (0 == ret)
	{
		printf("slm_init ok\n");
	}
	else
	{
		printf("slm_init error : 0x%08X\n", ret);
		goto CLEAR;
	}

	// 登录许可
	login_param.size = sizeof(ST_LOGIN_PARAM);
	login_param.license_id = 0;
	login_param.login_mode = SLM_LOGIN_MODE_AUTO;
	login_param.timeout = 600;

	// 登录访问许可ID
	ret = slm_login(&login_param, STRUCT, &(slm_handle), NULL);
	if (0 == ret)
	{
		printf("slm_login ok\n");
	}
	else
	{
		printf("slm_login error : 0x%08X\n", ret);
		goto CLEAR;
	}

	// 执行锁内代码输出计算后的结果
	ret = slm_execute_static(slm_handle, "h5first3.evx", (SS_BYTE*)lpToBeSigned,length, output, MAX_BUFFER_SIZE, &ret_len);
	if (ret != 0)
	{
		printf("slm_execute_static error:0x%08X\n", ret);
		goto CLEAR;
	}
	else
	{
		printf("slm_execute_static ok \n");
	}

CLEAR:
	// 许可使用完毕，登出许可，登出后不可执行与许可相关的操作。
	// 如仍需使用许可功能，请重新登录许可。
	if (0 != slm_handle)
	{
		ret = slm_logout(slm_handle);
		if (0 == ret)
		{
			printf("slm_logout ok.\n");
		}
		else
		{
			printf("slm_logout error : 0x%08X", ret);
		}
	}
	// 5.清空初始化申请资源（必须），全局调用一次即可。
	slm_cleanup();

	printf("ret_len:%u\n", ret_len);
	for (i = 0; i < ret_len; i++)
	{
		printf("%d", output[i]);
	}
	printf("\n");
	ret = SlcRsaVerify(SLC_PAD_MODE_PKCS_1_V1_5, SLC_HASH_ALGO_SHA1,pub_key,lpToBeSigned,length,output,ret_len);
	if(ret==SLC_SUCCESS)
	{
		printf("Verify success!\n");
	}
	else
	{
		printf("Verify error!\n");
		printf("error_code:0x%X\n", ret);
	}
	if (lpToBeSigned)
	{
		free(lpToBeSigned);
		lpToBeSigned = NULL;
	}
	system("pause");
	return 0;
	
}