/* 
  packet.h:the base data processed send to network
*/

/* 
  Copyright (C) 2008-2, Han.zhihong, Developer. Created 2008. 
  All rights reserved.

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

#pragma once
#ifndef _ZH_NET_C_SET_BIG_PACKET_

#ifdef __cplusplus
extern "C"{
#endif

#include "platform.h"
#include "gcdef.h"

/*
 *transmission data struct
 *
 *传输数据时打包的结构
*/
typedef struct _TzhPacketHeap
{
	unsigned short	wPos;
	unsigned short	wSize;
	unsigned char	*btBuf;
	int				nBufLenght;
}TzhPacketHeap;

/*
 *packet process function 
 *
 *数据包加解压处理函数
*/
void	zhPackHeapWriteInit(TzhPacketHeap *p,int packetLenght);
void	zhPackHeapReadInit(TzhPacketHeap *p,unsigned char*szBuf,int nBufSize);
void	zhPackHeapFree(TzhPacketHeap *p);

int		zhPackHeapWriteChar(TzhPacketHeap *p, char i);
int		zhPackHeapReadChar(TzhPacketHeap *p, char* i);
int		zhPackHeapWriteShort(TzhPacketHeap *p, short i);
int		zhPackHeapReadShort(TzhPacketHeap *p, short* i);
int		zhPackHeapWriteInt(TzhPacketHeap *p, int i);
int		zhPackHeapReadInt(TzhPacketHeap *p, int* i);
int		zhPackHeapWriteLong(TzhPacketHeap *p, long i);
int		zhPackHeapReadLong(TzhPacketHeap *p, long* i);
int		zhPackHeapWriteString(TzhPacketHeap *p, char* str);
int		zhPackHeapWriteStringUnicode(TzhPacketHeap *p, char* str);
int		zhPackHeapReadString(TzhPacketHeap *p, char* str);
int		zhPackHeapReadStringN(TzhPacketHeap *p, char* str ,int strMaxLen);
int		zhPackHeapReadStringUnicode(TzhPacketHeap *p, char* str);
int		zhPackHeapWriteBinary(TzhPacketHeap *p, char* str ,int len);
int		zhPackHeapReadBinary(TzhPacketHeap *p, char* str ,int len);
int		zhPackHeapWriteBool(TzhPacketHeap *p,bool b);
int		zhPackHeapReadBool(TzhPacketHeap *p, bool*b);
int		zhPackHeapWriteUnsignedChar(TzhPacketHeap *p,unsigned char i);
int		zhPackHeapReadUnsignedChar(TzhPacketHeap *p,unsigned char* i);
int		zhPackHeapWriteUnsignedShort(TzhPacketHeap *p,unsigned short i);
int		zhPackHeapReadUnsignedShort(TzhPacketHeap *p,unsigned short* i);
int		zhPackHeapWriteUnsignedInt(TzhPacketHeap *p,unsigned int i);
int		zhPackHeapReadUnsignedInt(TzhPacketHeap *p,unsigned int* i);
int		zhPackHeapWriteUnsignedLong(TzhPacketHeap *p,unsigned long i);
int		zhPackHeapReadUnsignedLong(TzhPacketHeap *p,unsigned long* i);

#ifdef __cplusplus
}
#endif
#define _ZH_NET_C_SET_BIG_PACKET_
#endif
