#include <malloc.h>
#include <stdio.h>

#include "sense4.h"
#include "common.h"

/*
	Print an array in Hex.	
	Parameters:
		fp[in]  : file pointer  
		data[in]: data to be printed
		len[in] : data length
		
	Return:
		none.

	Remarks:
		none.
*/
void hexprint(FILE *fp, void *data, int len)
{
	unsigned char *pdata = (unsigned char *)data;
	int i = 0;

	for (; i<len ;i++)
	{
		if (!(i%16))
			fprintf(fp,"\n");
		fprintf(fp, "%02x:",*(pdata+i));
	}
	fprintf(fp, "\n");
}

/*
	Select and open specified EliteIV device  
	Parameters:
		index[in]    : device index
		pstS4Ctx[out]: pointer to EliteIV context
		
	Return: 
		If the function succeeds, it will return 1(TRUE), otherwise 
		it returns 0(FALSE).

	Remarks:
		none.
*/
unsigned long OpenS4ByIndex(unsigned long index,PSENSE4_CONTEXT pstS4Ctx)
{
	PSENSE4_CONTEXT pstS4CtxList = NULL;
	unsigned long dwCtxListSize = 0;
	unsigned long dwResult = 0;
	unsigned long dwDeviceNum = 0;

	if (NULL == pstS4Ctx) 
	{
		printf("Invalid pointer!\n");
		return 0;
	}
	/*
		Use S4Enum(...) to Enumerate all the devices connected to the host.
		Using NULL as the 1st parameter will instruct S4Enum to return the buffer size needed to hold
		the corresponding EliteIV context in the 2nd parameter:dwCtxListSize.
		Generally only two results possible on return: (dwCtxListSize == 0) which means no EliteIV device 
		present or (dwCtxListSize != 0) and that's saying there are  dwCtxListSize/sizeof(SENSE4_CONTEXT) sets 
		of EliteIV devices connected currently.
	*/
	dwResult = S4Enum(NULL, &dwCtxListSize);
	if (dwResult != S4_SUCCESS && dwResult != S4_INSUFFICIENT_BUFFER)
	{
		printf("Enumerate EliteIV failed! <error code: 0x%08x>\n", dwResult);
		return 0;
	}

	if (0 == dwCtxListSize)
	{
		printf("EliteIV not found!\n");
		return 0;
	}

	// allocate memory for EliteIV context list/array
	pstS4CtxList = (PSENSE4_CONTEXT)malloc(dwCtxListSize);
	if (NULL == pstS4CtxList) 
	{
		printf("Not enough memory! \n");
		return 0;
	}
	
	dwDeviceNum = dwCtxListSize/sizeof(SENSE4_CONTEXT);
	if(index+1 > dwDeviceNum)
	{
		printf("Invalid index!<index should be within [0..%d]> \n",dwDeviceNum-1);
		free(pstS4CtxList);
		pstS4CtxList = NULL;
		return 0;
	}

	/*
		This time, call S4Enum(...) to do the real Enumeration.
		All the fields of the EliteIV context(s) pointed by pstS4CtxList will be filled 
		with the corresponding device information upon a successful function return.
	*/
	dwResult = S4Enum(pstS4CtxList, &dwCtxListSize);
	if (dwResult != S4_SUCCESS) 
	{
		printf("Enumerate EliteIV failed! <error code: 0x%08x>\n", dwResult);
		free(pstS4CtxList);
		pstS4CtxList = NULL;
		return 0;
	}

	// use device with specified index  
	memcpy(pstS4Ctx, pstS4CtxList+index, sizeof(SENSE4_CONTEXT));

	free(pstS4CtxList);
	pstS4CtxList = NULL;

	// Call S4Open(...) to open the specified EliteIV device in "share mode".
	dwResult = S4Open(pstS4Ctx);
	if (dwResult != S4_SUCCESS) 
	{
		printf("Open EliteIV failed! <error code: 0x%08x>\n", dwResult);
		return 0;
	}
	return 1;
}
/*
	Reset and close specified EliteIV device  
	Parameters:
		pstS4Ctx[out]: pointer to EliteIV context
		
	Return: 
		none.	

	Remarks:
		Use this function instead of S4Close() may enhance security in certain
		circumstances.
*/
void ResetAndCloseS4(PSENSE4_CONTEXT pstS4Ctx)
{
	S4Control(pstS4Ctx, S4_RESET_DEVICE, NULL, 0, NULL, 0, NULL);
	S4Close(pstS4Ctx);
	return ;
}
