#ifndef __SAMPLE_RSA_H__
#define __SAMPLE_RSA_H__

//By default, program compiled at IBM-compatible PC	has the address alignment issues which don't exist 
//in C51 environment. For convenience of data-handling,we use the compact-pack mode for storing the
//exchanging data-buffer used between PC and EliteIV device.For more ,pls refer to sample 17.

#ifdef _WIN32
#pragma pack(push, 1)
#endif 

#define RSA_BLOCK_LEN		0x80 // RSA block length
#define IO_PACKAGE_HEADER_SIZE 2 //IO package header length:tag+len
 
#define DA_MD2 3 // digest algorithm:MD2              
#define DA_MD5 5 // digest algorithm:MD5              
#define DA_SHS 0x1a // digest algorithm:SHA1

#define SHA1_BLOCK_LEN      0x14 // digest block length:SHA1
#define MD5_BLOCK_LEN       0x10 // digest block length:MD5
#define MAX_HASH_BLOCK_LEN  0x14 // maximum digest block length possible

#define MAX_IO_DATA_SIZE	0xf8 // maximum IO data size

/* command group definition */
#define CMD_SIGN	0x01 
#define CMD_VERIFY	0x02
#define CMD_ENCRYPT 0x03
#define CMD_DECRYPT	0x04

/* common error code. */
#define ERR_SUCCESS				0x00	// success
#define ERR_SES					0x80	// SES function execution error
#define ERR_INVALID_PARAMETER	0x81	// invalid parameter


/* user-specific error code */
#define ERR_INCORRECT_SIGNATURE 0x90	// incorret signature

/* user-specific data block */
typedef struct _RSA_BLOCK
{							                                                                                               
	unsigned short fid; // File ID of the private/public key.
	unsigned char dgst_alg; // digest algorithm
	unsigned char len; 
	unsigned char buff[RSA_BLOCK_LEN];
} RSA_BLOCK;

typedef struct _RSA_VERIFY_BLOCK
{							                                                                                               
	unsigned short fid; // File ID of the public key.
	unsigned char dgst_alg;
	unsigned char dgst[MAX_HASH_BLOCK_LEN];
	unsigned char dgst_len; 
	unsigned char sig[RSA_BLOCK_LEN];
	unsigned char sig_len;
} RSA_VERIFY_BLOCK;

/* IO package */
typedef struct _IO_PACKAGE
{	
	unsigned char tag;	
	unsigned char len;
	unsigned char buff[MAX_IO_DATA_SIZE];
} IO_PACKAGE;

#ifdef _WIN32
#pragma pack(pop)
#endif

#endif //__SAMPLE_RSA_H__