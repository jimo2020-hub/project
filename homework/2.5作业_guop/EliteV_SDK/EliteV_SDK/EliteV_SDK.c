#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include "ss_error.h"
#include "ss_define.h"
#include "ss_lm_runtime.h"
#include "common.h"

#if _MSC_VER>=1900
#include "stdio.h" 
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#ifdef __cplusplus 
extern "C"
#endif

FILE* __cdecl __iob_func(unsigned i) {
    return __acrt_iob_func(i);
}
#endif /* _MSC_VER>=1900 */

static int back_round_check = 0;
char hex[1024 * 1024 * 10] = { 0 };
unsigned char Byte[1024 * 1024 * 10] = { 0 };
SLM_HANDLE_INDEX slm_handle = 0;
ST_LOGIN_PARAM login_param    = {0};

int char_to_int(char c)
{
	int ret = 0;
	switch (c)
	{
	case'0':
		ret = 0; break;
	case 0:
		ret = 0; break;
	case'1':
		ret = 1; break;
	case'2':
		ret = 2; break;
	case'3':
		ret = 3; break;
	case'4':
		ret = 4; break;
	case'5':
		ret = 5; break;
	case'6':
		ret = 6; break;
	case'7':
		ret = 7; break;
	case'8':
		ret = 8; break;
	case'9':
		ret = 9; break;
	case('A'):
		ret = 10; break;
	case('a'):
		ret = 10; break;
	case('B'):
		ret = 11; break;
	case('b'):
		ret = 11; break;
	case('C'):
		ret = 12; break;
	case('c'):
		ret = 12; break;
	case('D'):
		ret = 13; break;
	case('d'):
		ret = 13; break;
	case('E'):
		ret = 14; break;
	case('e'):
		ret = 14; break;
	case('F'):
		ret = 15; break;
	case('f'):
		ret = 15; break;
	default:
		return -1;
	}
	return ret;
}

int hex_to_byte(char* hex,unsigned char *byte)
{
	int sscanfbyte = 0;
	int hex_size = strlen(hex);
	int i= 0;
	int tmp_low;
	int tmp_high;
	int tmp;
	char hex_low;
	char hex_high;

	for (i = 0; i < hex_size; i = i + 2)
	{
		hex_low = hex[i + 1];
		hex_high = hex[i];
		tmp_low = char_to_int(hex_low);
		tmp_high = char_to_int(hex_high);

		if ((-1 == tmp_low) | (-1 == tmp_high))
		{
			return -1;
		}

		if ((i + 1 == hex_size) && (0 == hex[hex_size]))
		{
			tmp = tmp_low;
			tmp_low = tmp_high;
			tmp_high = tmp;
		}

		tmp = tmp_low | tmp_high << 4;
		byte[sscanfbyte] = tmp;
		sscanfbyte++;
	}

	return sscanfbyte;
}
DWORD WINAPI thread_keep_alive(VOID * param)
{
    SS_UINT32 ret = 0;
    SLM_HANDLE_INDEX pe5_handle = 0;
	SLM_HANDLE_INDEX rslm_handle = 0;
    pe5_handle = slm_handle;

    while(1)
    {
        ret = slm_keep_alive(pe5_handle); 
	    if(SS_OK != ret)
	    {
		    printf("thread:slm_keep_alive error : 0x%08X\n", ret);
			ret=slm_relogin(login_param,&rslm_handle);
			pe5_handle=rslm_handle;
			if(SS_OK==ret)
			{
				printf("relogin success!\n");
			}
	    }
	    else
	    {
		    printf("thread:slm_keep_alive ok\n");
            Sleep(1000*back_round_check);
	    }
    }

    return 0;
}


/**
������Ϣ����ص�����

�ܹ����յ�Ӳ�����β���Ϣ��������ͣ����Ϣ��
 */
#define DEVICE_SN_LENGTH 16
SS_UINT32 SSAPI handle_service_msg( SS_UINT32 message, void* wparam, void* lparam )
{
    SS_UINT32 ret = SS_OK;
    SS_UINT32 result = 0;
    char szmsg[1024]={0};
    char lock_sn[DEVICE_SN_LENGTH] = {0};
    char szlock_sn[DEVICE_SN_LENGTH*2 + 1] = {0};

    switch(message)
    {
		case SS_MSG_LOCK_AVAILABLE:     // �����ã���������SS����ʱ���ѳ�ʼ����ɣ���wparam ��������
			// ��������Ϣ�����Ը������Ų�ѯ���������Ϣ��ʵ���Զ���¼����ȹ��ܡ�

			// �������Ƽ�¼������ת����ʮ�������ַ���
			memcpy(lock_sn, wparam, DEVICE_SN_LENGTH);
			bytes_to_hexstr((unsigned char*)lock_sn, DEVICE_SN_LENGTH, szlock_sn);

			sprintf(szmsg, "time is %08u, SS_MSG_LOCK_AVAILABLE is 0x%08X wparam is locksn -memory address %p, lock-sn is %s", 
				GetTickCount(), message, wparam, szlock_sn);
			break;
		case SS_MSG_LOCK_UNAVAILABLE:   // ����Ч�����Ѱγ�����wparam ��������
			// ���γ���Ϣ������ֻʹ������Ӧ�ó���һ���������γ�������޷�����ʹ�ã����鷢�ִ���Ϣ��ʾ�û��������ݣ������������Ȳ�����

			// �������Ƽ�¼������ת����ʮ�������ַ���
			memcpy(lock_sn, wparam, DEVICE_SN_LENGTH);
			bytes_to_hexstr((unsigned char*)lock_sn, DEVICE_SN_LENGTH, szlock_sn);

			sprintf(szmsg, "time is %08u SS_MSG_LOCK_UNAVAILABLE is 0x%08X wparam is locksn -memory address %p, lock-sn is %s", 
				GetTickCount(), message, wparam, szlock_sn);
			break;
    }    
    // �����ʽ�������Ϣ����
    printf("%s\n", szmsg);
    return ret;
}
SS_UINT32 slm_relogin(ST_LOGIN_PARAM login_param,SLM_HANDLE_INDEX *rslm_handle)
{
	SS_UINT32 ret = SS_OK;
	ret = slm_login(&login_param, STRUCT, &(slm_handle), NULL);
	if(ret==SS_OK)
	{
		printf("relogin success!\n");
	}
	else if(SS_ERROR_LIC_NOT_FOUND == ret)
	{
		printf("relogin error : 0x%08X(SS_ERROR_LIC_NOT_FOUND), license_id = %u\n", ret, login_param.license_id);
		return ret;
	}
	else
	{
		printf("relogin error : 0x%08X\n", ret);
		return ret;
	}
	*rslm_handle=slm_handle;
}
int main()
{
    SS_UINT32 ret = SS_OK;
    ST_INIT_PARAM init_param = {0};
    char *license_info_json = NULL;
	char key_val=0;
    int sscanfbyte,hex_size = 0;
    HANDLE hthread = NULL;
	HANDLE hthread1 = NULL;
    DWORD dwThreadID = 0;
    DWORD dwret = 0; 

loop_api:
	printf("please input api_password\n");
	scanf("%32s",hex);
	sscanfbyte=hex_to_byte(hex,Byte);
	if(-1==sscanfbyte)
	{
		printf("The input is wrong, please re-enter!\n");
		goto loop_api;
	}
	memcpy(init_param.password, Byte,sscanfbyte);
	
	printf("please input license id\n");
	scanf("%u",&login_param.license_id);

loop_sn:
	printf("please input lock -sn\n");
	scanf("%32s",hex);
	sscanfbyte=hex_to_byte(hex,Byte);
	if((-1==sscanfbyte)|(sscanfbyte!=16))
	{
		printf("The input is wrong, please re-enter!\n");
		goto loop_sn;
	}
	memcpy(login_param.sn, Byte,sscanfbyte);

	do
	{
		printf("please input login mode����������local,remote,auto,cloud,slock�ȼ����ַ�����\n");
		scanf("%6s",hex);
		if(strncmp(hex,"local",5)==0)
		{
			login_param.login_mode=SLM_LOGIN_MODE_AUTO;
		}
		else if(strncmp(hex,"remote",6)==0)
		{
			login_param.login_mode=SLM_LOGIN_MODE_REMOTE;
		}
		else if(strncmp(hex,"auto",4)==0)
		{
			login_param.login_mode=SLM_LOGIN_MODE_AUTO;
		}
		else if(strncmp(hex,"cloud",5)==0)
		{
			login_param.login_mode=SLM_LOGIN_MODE_CLOUD;
		}
		else if(strncmp(hex,"slock",5)==0)
		{
			login_param.login_mode=SLM_LOGIN_MODE_SLOCK;
		}
		else
		{
			login_param.login_mode=0x1000;
		}
	}
	while(login_param.login_mode==0x1000);

    printf("please input backround check dongle time-interleve\n");
	scanf("%d",&back_round_check);
	if(0==back_round_check)
	{
		goto CLEAR;
	}
	if(back_round_check>600)
	{
		back_round_check=600;
	}

	//��ʼ��init_param�ṹ��,ִ��slm_init
    init_param.version = SLM_CALLBACK_VERSION02;
	init_param.pfn = handle_service_msg;
    init_param.flag = SLM_INIT_FLAG_NOTIFY;
    init_param.timeout = 0;
    
    ret = slm_init(&(init_param));
    if(SS_OK == ret)
    {
        printf("slm_init ok\n");
    }
    else if(SS_ERROR_DEVELOPER_PASSWORD == ret)
    {
        printf("slm_init error : 0x%08X(SS_ERROR_DEVELOPER_PASSWORD), Please login to the Virbox Developer Center(https://developer.lm.virbox.com), get the API password, and replace the 'g_api_password' variable content.\n", ret);
        goto CLEAR;
    }
    else
    {
        printf("slm_init error : 0x%08X\n", ret);
        goto CLEAR;
    }

	//��ʼ��login_param�ṹ�壬ִ��slm_login
    login_param.size = sizeof(ST_LOGIN_PARAM);
    login_param.timeout = 600;
    ret = slm_login(&login_param, STRUCT, &(slm_handle), NULL);
    if(SS_OK == ret)
    {
        printf("slm_login ok\n");
    }
    else if(SS_ERROR_LIC_NOT_FOUND == ret)
    {
        printf("slm_login error : 0x%08X(SS_ERROR_LIC_NOT_FOUND), license_id = %u\n", ret, login_param.license_id);
        goto CLEAR;
    }
    else
    {
        printf("slm_login error : 0x%08X\n", ret);
        goto CLEAR;
    }

    // ��ȡ�����Ϣ��LICENSE_INFO����ָ������JSON��ʽ���ݡ�
    ret = slm_get_info(slm_handle, LICENSE_INFO, JSON, &license_info_json);
    if(SS_OK != ret)
    {
        printf("slm_get_info[LICENSE_INFO] error : 0x%08X\n", ret);
        goto CLEAR;
    }
    else
    {
        printf("[LICENSE_INFO]:\n%s\n", license_info_json);
    }

    // ��Ҫ�������ͷ��ڴ棬ʹ����ϲ��ͷŻ�����ڴ�й¶��
    slm_free(license_info_json);
    license_info_json = NULL;

	//��̨ѭ����⣬���ʱ����1-600s֮��
    hthread = CreateThread(NULL,0,thread_keep_alive,NULL,CREATE_SUSPENDED,&dwThreadID);
    ResumeThread(hthread);
	printf("������Q��q�˳�������\n");
	while(1)
	{
		key_val=getchar();
		if((key_val=='Q')||(key_val=='q'))
		{
			printf("demo������byebye\n");
			break;
		}
	}

CLEAR:
    // 4.���ʹ����ϣ��ǳ���ɣ��ǳ��󲻿�ִ���������صĲ�����
    // ������ʹ����ɹ��ܣ������µ�¼��ɡ�
    if ( 0 != slm_handle )
    {
        ret = slm_logout(slm_handle);
        if (SS_OK == ret) 
        {
            printf("slm_logout ok.\n");
        }
        else
        {
            printf("slm_logout error : 0x%08X\n", ret);
        }
    }
	ret=slm_cleanup();

	if(SS_OK == ret)
	{
		printf("slm_cleanup success\n");
	}
	else
	{
		printf("slm_cleanup error\n");
	}
    printf("\npress any key exit process.\n");
    return 0;
}