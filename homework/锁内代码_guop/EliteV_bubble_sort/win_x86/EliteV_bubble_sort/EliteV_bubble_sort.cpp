#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "ss_lm_runtime.h"
#include "ss_define.h"

#define MAX_BUFFER_SIZE 1024
#define N 5

int main(int argc, char* argv[])
{
	int input[N] = { 0 };
	int input_len = sizeof(input);
	int i;
	
	printf("******请输入%d个整数以“ ”作为分割符******\n", N);
	for (i = 0; i < N; i++)	
	{
		scanf_s("%d", &input[i]);
	}
	
	SS_UINT32 ret = 0;
	ST_INIT_PARAM init_param = { 0 };
	ST_LOGIN_PARAM login_param = { 0 };
	SLM_HANDLE_INDEX slm_handle = 0;
	SS_UINT32 ret_len = 0;
	init_param.version = SLM_CALLBACK_VERSION02;
	init_param.pfn = NULL;
	init_param.timeout = 0;
	SS_BYTE output[MAX_BUFFER_SIZE] = { 0 };
	int result[N] = { 0 };
	int *presult = NULL;

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
	ret = slm_execute_static(slm_handle, "h5first.evx", (SS_BYTE*)input, input_len, output, MAX_BUFFER_SIZE, &ret_len);
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

	/*指针指向缓冲区的值*/
	presult = (int*)&(output[0]);
	printf("Output Value:\n");
	for (i = 0; i < ret_len / sizeof(int); i++)
	{
		printf("%d ", *presult);
		presult++;
	}
	printf("\n");
	return 0;
}