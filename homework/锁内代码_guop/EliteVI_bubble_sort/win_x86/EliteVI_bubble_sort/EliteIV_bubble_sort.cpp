#include <stdio.h>
#include "sense4.h"
#include <stdlib.h>
#include <conio.h>
#include <vld.h>
#define N 9


union IntToFloat
{
    int m_i32;
    float m_f32;
};

union IntToFloat i;
/******int类型大小端转换*******/
int swapInt32(int value)
{
    return ((value & 0x000000FF) << 24) |
        ((value & 0x0000FF00) << 8) |
        ((value & 0x00FF0000) >> 8) |
        ((value & 0xFF000000) >> 24);
}

int main(int argc, char* argv[])
{
    long test[N] = { 11111111,22222222,33333333,44444444,55555555,66666666,77777777,88888888,99999999};
    long StoL[N] = { 0 };
    int len = sizeof(test);
	int j=0;
    SENSE4_CONTEXT ctx = { 0 };
    SENSE4_CONTEXT* pctx = NULL;
    unsigned long size = 0;
    unsigned long ret = 0;
    unsigned char fid[] = "0004";//精锐4锁内执行文件，用于冒泡排序
    

    for (j = 0; j < N; j++)
    {
        StoL[j] = swapInt32(test[j]);
    }
    /*列举精锐 4S 设备*/
    S4Enum(pctx, &size);
    if (size == 0)
    {
        printf("Elite4S not found!\n");
        return -1;
    }
    pctx = (SENSE4_CONTEXT*)malloc(size);
    if (pctx == NULL)
    {
        printf("Not enough memory!\n");
        return -1;
    }
    ret = S4Enum(pctx, &size);
    if (ret != S4_SUCCESS)
    {
        printf("Enumerate Elite4S error!\n");
        free(pctx);
        return -1;
    }
    memcpy(&ctx, pctx, sizeof(SENSE4_CONTEXT));
    free(pctx);
    pctx = NULL;
    /*打开精锐 4S 设备*/
    ret = S4Open(&ctx);
    if (ret != S4_SUCCESS)
    {
        printf("Open Elite4S failed!\n");
        return -1;
    }
    /*切换到根目录*/
    ret = S4ChangeDir(&ctx, "\\");
    if (ret != S4_SUCCESS)
    {
        printf("No root directory found!\n");
        S4Close(&ctx);
        return -1;
    }
    /*验证根目录下的用户 PIN 码*/
    ret = S4VerifyPin(&ctx, (BYTE*)"12345678", 8, S4_USER_PIN);
    if (ret != S4_SUCCESS)
    {
        printf("Verify user PIN failed!\n");
        S4Close(&ctx);
        return -1;
    }
    printf("执行锁内代码前的结果:\n");
    for (j = 0; j < len/4; j++)
    {
        printf("%ld ", test[j]);
    }
    printf("\n");
    /*执行冒泡排序的可执行文件 0001*/
    ret = S4Execute(&ctx, (LPCSTR)fid, StoL, len, test, len, &size);
    if (ret != S4_SUCCESS)
    {
        printf("Execute Elite4S exe failed!\n");
        S4Close(&ctx);
        return -1;
    }
    S4Close(&ctx);
    for (j = 0; j < len/4; j++)
    {
        test[j] = swapInt32(test[j]);
    }
    printf("执行锁内代码后的结果:\n");
    for (j = 0; j < len / 4; j++)
    {
        printf("%ld ", test[j]);
    }
    printf("\n");
    return 0;
}
