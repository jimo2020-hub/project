#include <stdio.h>
#include <vld.h>
#include "common.h"
#include "ss_define.h"
#include "ss_error.h"
#include "ss_lm_runtime.h"
#include "slmCrypto.h"
#include "cJSON.h"
#if _MSC_VER>=1900
#include "stdio.h" 
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#ifdef __cplusplus 
extern "C"
#endif
FILE* __cdecl __iob_func(unsigned i) {
    return __acrt_iob_func(i);
}
#endif
#define MAX_USER_DATA_SIZE 64 * 1024
#define TEMP_SIZE 256
#define ULPUBLEN_LEN 256
#define SN_LEN 256
#define SIGN_LEN 256

unsigned char pub_key[152] = {  
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

int main(int argc,char *argv[])
{
    SS_UINT32 ret = SS_OK;
    ST_INIT_PARAM init_param = { 0 };
    ST_LOGIN_PARAM login_param = { 0 };
    SLM_HANDLE_INDEX slm_handle[TEMP_SIZE] = {0};
    SS_BYTE *pData = NULL;
    SS_UINT32 ulPUBLen[ULPUBLEN_LEN] = {0};    
    SS_UINT32 ulRAWLen = 0;
    SS_CHAR temp[TEMP_SIZE] = {0};
    SS_CHAR *sn[SN_LEN] = {0};
    SLC_BYTE sign_a[SIGN_LEN] ={0};//存储a锁的公开区的签名数据
    SLC_BYTE sign_b[SIGN_LEN] = {0};//存储b锁的公开区的签名数据
    FILE *fp = NULL;
    SS_CHAR *pchar_Devdescs    = NULL;
    cJSON *pjson_Devdescs = NULL;
    SS_UINT32 dev_num = 0;
    cJSON *pjson_Array = NULL;
    cJSON *pjson_Item = NULL;
    SS_UINT32 sn_len = 32;
    SS_UINT32 i = 0;
    SS_UINT32 index_device = 0;

    //初始化init_param结构体，执行slm_init
    init_param.version = SLM_CALLBACK_VERSION02;
    init_param.timeout = 0;
    memcpy(init_param.password, g_api_password, sizeof(g_api_password));

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

    //枚举设备
    ret = slm_enum_device( &pchar_Devdescs );
    //将返回的设备描述解析成JSON
    pjson_Devdescs = cJSON_Parse(pchar_Devdescs);
    if (!pjson_Devdescs)
    {
        if(pjson_Devdescs)
        {
            free(pjson_Devdescs);
            pjson_Devdescs = NULL;
        }
        printf("cJSON_Parse error\n");
        goto CLEAR;
    }
    //获取设备数
    dev_num    = cJSON_GetArraySize( pjson_Devdescs );
    printf("device count : %d\n", dev_num );
    //获取设备锁号
    for(index_device = 0; index_device < dev_num; index_device++ )
    {
        printf("device %d[device-desc]:\n", index_device);
        pjson_Array = cJSON_GetArrayItem( pjson_Devdescs, index_device );
        pjson_Item = cJSON_GetObjectItem( pjson_Array, "sn" );
        printf("type : %s\n",pjson_Item->valuestring);
        sn[index_device] = pjson_Item->valuestring;
    }

    //初始化login_param结构体，执行slm_login
    login_param.size = sizeof(ST_LOGIN_PARAM);
    login_param.license_id = 23;
    login_param.login_mode = SLM_LOGIN_MODE_LOCAL;
    login_param.login_flag = SLM_LOGIN_FLAG_LOCKSN;
    login_param.timeout = 600;
    for(i=0;i<dev_num;i++)
    {
        hexstr_to_bytes(sn[i], sn_len, (unsigned char *)temp);
        memcpy(login_param.sn,temp,16);

        ret = slm_login(&login_param,STRUCT,&(slm_handle[i]),NULL);
        if (0 == ret)
        {   
            printf("slm_login ok\n");
        }      
        else
        {
            printf("slm_login error : 0x%08X\n", ret);
            goto CLEAR;
        }
        ret = slm_user_data_getsize(slm_handle[i], PUB, &ulPUBLen[i]);
        if(SS_OK != ret)
        {
            printf("slm_user_data_getsize error!\n");
            goto CLEAR;
        }
    }

    if(dev_num!=0)
    {
        if(ulPUBLen[0] > ulPUBLen[1])
        {
            //获取读写区大小
            ret = slm_user_data_getsize(slm_handle[0], RAW, &ulRAWLen);
            if(SS_OK != ret)
            {
                printf("slm_user_data_getsize[RAW] error : 0x%08X\n", ret);
                goto CLEAR;
            }
            else
            {
                printf( "slm_user_data_getsize[RAW] ok ram_size : %d\n", ulRAWLen );
            } 

            memset(sign_a, 0, sizeof(sign_a));
            if(ulRAWLen <= MAX_USER_DATA_SIZE)
            {
                //获取读写区数据
                ret = slm_user_data_read(slm_handle[0], RAW, sign_a, 0, ulRAWLen); 
                if(SS_OK != ret)
                {
                    printf("slm_user_data_read[RAW][SHORT] error : 0x%08X\n", ret);
                    goto CLEAR;
                } 
            }

            ret = slm_user_data_getsize(slm_handle[0], PUB, &ulPUBLen[0]);
            if (ret == SS_OK && ulPUBLen[0] > 0)
            {    
        
                pData = (SS_BYTE *)calloc(sizeof(SS_BYTE), ulPUBLen[0]);  
                //读取公开区数据
                ret = slm_user_data_read(slm_handle[0], PUB, pData, 0, ulPUBLen[0]); 

                if(SS_OK != ret)
                {
                    printf("slm_user_data_read[pub] error : 0x%08x\n", ret);
                    goto CLEAR;
                } 

                if((fp=fopen("./2.jpg", "wb")) == NULL)
                {
                    perror("txtxFile opening failed\n");
                    return -1;
                }
                //验证公开区数据签名
                ret = SlcRsaVerify(SLC_PAD_MODE_PKCS_1_V1_5, SLC_HASH_ALGO_SHA256,pub_key,pData,ulPUBLen[0],sign_a,128);
                if( SLC_SUCCESS == ret)
                {
                    printf("Verify lock_a Success!\n");
                    fwrite(pData,1,ulPUBLen[0], fp);
                    fclose(fp);
                }
                else
                {
                    printf("SlcRsaVerify Error!\n");
                    fclose(fp);
                    free(pData);
                    pData = NULL;    
                    return -1;
                }    
                free(pData);        
                pData = NULL;                
            }

            ulRAWLen = 0;
            ret = slm_user_data_getsize(slm_handle[1], RAW, &ulRAWLen);
            if(SS_OK != ret)
            {
                printf("slm_user_data_getsize[RAW] error : 0x%08X\n", ret);
                goto CLEAR;
            }
            else
            {
                printf( "slm_user_data_getsize[RAW] ok ram_size : %d\n", ulRAWLen );
            } 

            memset(sign_b, 0, sizeof(sign_b));
            if(ulRAWLen <= MAX_USER_DATA_SIZE)
            {
                ret = slm_user_data_read(slm_handle[1], RAW, sign_b, 0, ulRAWLen); 
                if(SS_OK != ret)
                {
                    printf("slm_user_data_read[RAW][SHORT] error : 0x%08X\n", ret);
                    goto CLEAR;
                } 
            }

            ret = slm_user_data_getsize(slm_handle[1], PUB, &ulPUBLen[1]);
            if (ret == SS_OK && ulPUBLen[1] > 0)
            {    
                pData = (SS_BYTE *)calloc(sizeof(SS_BYTE), ulPUBLen[1]);        
                ret = slm_user_data_read(slm_handle[1], PUB, pData, 0, ulPUBLen[1]); 
                if(SS_OK != ret)
                {
                    printf("slm_user_data_read[pub] error : 0x%08x\n", ret);
                    goto CLEAR;
                } 

                if((fp=fopen("./2.jpg", "ab")) == NULL)
                {
                    perror("txtxFile opening failed\n");
                    return -1;
                }

                ret = SlcRsaVerify(SLC_PAD_MODE_PKCS_1_V1_5, SLC_HASH_ALGO_SHA256,pub_key,pData,ulPUBLen[1],sign_b,128);
                if( SLC_SUCCESS == ret)
                {
                    printf("Verify lock_b Success!\n");
                    fwrite(pData,1,ulPUBLen[1], fp);
                    fclose(fp);
        
                }
                else
                {
                    printf("SlcRsaVerify Error!\n");
                    fclose(fp);
                    free(pData);        
                    pData = NULL;
                    return -1;
                }
                    free(pData);        
                pData = NULL;        
            }
        }
        else
        {
            ret = slm_user_data_getsize(slm_handle[1], RAW, &ulRAWLen);
            if(SS_OK != ret)
            {
                printf("slm_user_data_getsize[RAW] error : 0x%08X\n", ret);
                goto CLEAR;
            }
            else
            {
                printf( "slm_user_data_getsize[RAW] ok ram_size : %d\n", ulRAWLen );
            } 

            memset(sign_a, 0, sizeof(sign_a));
            if(ulRAWLen <= MAX_USER_DATA_SIZE)
            {
                ret = slm_user_data_read(slm_handle[1], RAW, sign_a, 0, ulRAWLen); 
                if(SS_OK != ret)
                {
                    printf("slm_user_data_read[RAW][SHORT] error : 0x%08X\n", ret);
                    goto CLEAR;
                } 
            }

            ret = slm_user_data_getsize(slm_handle[1], PUB, &ulPUBLen[1]);
            if (ret == SS_OK && ulPUBLen[1] > 0)
            {    
        
                pData = (SS_BYTE *)calloc(sizeof(SS_BYTE), ulPUBLen[1]); 

                ret = slm_user_data_read(slm_handle[1], PUB, pData, 0, ulPUBLen[1]); 
                if(SS_OK != ret)
                {
                    printf("slm_user_data_read[pub] error : 0x%08x\n", ret);
                    goto CLEAR;
                } 

                if((fp=fopen("./2.jpg", "wb")) == NULL)
                {
                    perror("txtxFile opening failed\n");
                    return -1;
                }

                ret = SlcRsaVerify(SLC_PAD_MODE_PKCS_1_V1_5, SLC_HASH_ALGO_SHA256,pub_key,pData,ulPUBLen[1],sign_a,128);
                if( SLC_SUCCESS == ret)
                {
                    printf("Verify lock_a Success!\n");
                    fwrite(pData,1,ulPUBLen[1], fp);
                    fclose(fp);
                }
                else
                {
                    printf("SlcRsaVerify Error!\n");
                    fclose(fp);
                    free(pData);
                    pData = NULL;    
                    return -1;
                }    
                free(pData);        
                pData = NULL;                
            }

            ulRAWLen = 0;
            ret = slm_user_data_getsize(slm_handle[0], RAW, &ulRAWLen);
            if(SS_OK != ret)
            {
                printf("slm_user_data_getsize[RAW] error : 0x%08X\n", ret);
                goto CLEAR;
            }
            else
            {
                printf( "slm_user_data_getsize[RAW] ok ram_size : %d\n", ulRAWLen );
            } 

            memset(sign_b, 0, sizeof(sign_b));
            if(ulRAWLen <= MAX_USER_DATA_SIZE)
            {
                ret = slm_user_data_read(slm_handle[0], RAW, sign_b, 0, ulRAWLen); 
                if(SS_OK != ret)
                {
                    printf("slm_user_data_read[RAW][SHORT] error : 0x%08X\n", ret);
                    goto CLEAR;
                } 
            }

            ret = slm_user_data_getsize(slm_handle[0], PUB, &ulPUBLen[0]);
            if (ret == SS_OK && ulPUBLen[1] > 0)
            {    
                pData = (SS_BYTE *)calloc(sizeof(SS_BYTE), ulPUBLen[0]);        
                ret = slm_user_data_read(slm_handle[0], PUB, pData, 0, ulPUBLen[0]); 
                if(SS_OK != ret)
                {
                    printf("slm_user_data_read[pub] error : 0x%08x\n", ret);
                    goto CLEAR;
                } 

                if((fp=fopen("./2.jpg", "ab")) == NULL)
                {
                    perror("txtxFile opening failed\n");
                    return -1;
                }

                ret = SlcRsaVerify(SLC_PAD_MODE_PKCS_1_V1_5, SLC_HASH_ALGO_SHA256,pub_key,pData,ulPUBLen[0],sign_b,128);
                if( SLC_SUCCESS == ret)
                {
                    printf("Verify lock_b Success!\n");
                    fwrite(pData,1,ulPUBLen[0], fp);
                    fclose(fp);
        
                }
                else
                {
                    printf("SlcRsaVerify Error!\n");
                    fclose(fp);
                    free(pData);        
                    pData = NULL;
                    return -1;
                }
                free(pData);        
                pData = NULL;        
            }
        }
    }


    
CLEAR:
    // 许可使用完毕，登出许可，登出后不可执行与许可相关的操作。
    // 如仍需使用许可功能，请重新登录许可。
    for(i = 0;i < dev_num;i++)
    {
        if (0 != slm_handle[i])
        {
            ret = slm_logout(slm_handle[i]);
            if (0 == ret)
            {
                printf("slm_logout ok.\n");
            }
            else
            {
                printf("slm_logout error : 0x%08X", ret);
            }
        }
    }

    // 5.清空初始化申请资源（必须），全局调用一次即可。
    slm_cleanup();
    system("pause");
    return 0;
    
}