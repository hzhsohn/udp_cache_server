/* encrypt.c - Data Security, custom message-digest algorithm
*/

/* Copyright (C) 2008-2, Han.zhihong, Developer. Created 2008. All
rights reserved.

  License to copy and use this software is granted provided that it
  is identified as the "Han.zhihong. Message-Digest
  Algorithm" in all material mentioning or referencing this software
  or this function.
  
  Han.zhihong. makes no representations concerning either
  the merchantability of this software or the suitability of this
  software for any particular purpose. It is provided "as is"
  without express or implied warranty of any kind.
	  
  These notices must be retained in any copies of any part of this
  documentation and/or software.
*/

#include "encrypt.h"

//
//Initial start random key
//
//初始始化随机钥匙
//
int zhNetGetRandEncryptKey()
{
	time_t dw=0;
	int ret=0;
	dw=zhPlatGetTime();
	srand((int)dw);
	ret=rand();
	ret|=rand()<<24;
	ret|=rand()<<16;
	while(ret == 0)
	{
		dw=zhPlatGetTime();
		srand((int)dw);
		ret = rand();
	}
	return ret;
}

//
//sub function,using in this moudle
//function for encrypt data
//
bool zhNetEncrypt(bool isNeedEncrypt,char* buf, int len,unsigned char nEncryptKey[4])
{
	if(isNeedEncrypt)
	{
		int i,j;
		for(i=0; i<len; i++)
		{
			for(j=0; j<4; j++)
			{
				buf[i] = buf[i]^nEncryptKey[j];
			}
		}
	}
	return true;
}

//
//sub function,using in this moudle
//decrypt data
//
bool zhNetDecrypt(bool isNeedEncrypt,char* buf, int len,unsigned char nEncryptKey[4])
{
	if(isNeedEncrypt)
	{
		int i,j;
		for(i=0; i<len; i++)
		{
			for(j=3; j>=0; j--)
			{
				buf[i] = buf[i]^nEncryptKey[j];
			}
		}
	}
	return true;
}
