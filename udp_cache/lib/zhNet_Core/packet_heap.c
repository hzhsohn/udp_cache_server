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

#include <memory.h>
#include <string.h>
#include "packet_heap.h"

//
//initizal read data
//
//初始化读取数据
//
void zhPackHeapReadInit(TzhPacketHeap *p,unsigned char*szBuf,int nBufSize)
{
	p->wPos=0;
	p->wSize=nBufSize;
	p->btBuf=(unsigned char*)malloc(nBufSize);
	memcpy(p->btBuf,szBuf,nBufSize);
	p->nBufLenght=nBufSize;
}
void zhPackHeapWriteInit(TzhPacketHeap *p,int packetLenght)
{
	p->wPos=0;
	p->wSize=0;
	p->btBuf=(unsigned char*)malloc(packetLenght);
	memset(p->btBuf,0,packetLenght);
	p->nBufLenght=packetLenght;
}
void zhPackHeapFree(TzhPacketHeap *p)
{
	p->wPos=0;
	p->wSize=0;
	if(p->btBuf)
	{
		free(p->btBuf);
		p->btBuf=NULL;
	}
}

int zhPackHeapWriteChar(TzhPacketHeap *p, char i)
{
	if((int)(p->wPos+sizeof(char))>p->nBufLenght){return 0;}
	memcpy(&p->btBuf[(int)p->wPos], &i, sizeof(char));
	p->wPos+=sizeof(char);
	p->wSize+=sizeof(char);
	return sizeof(char);
}
int zhPackHeapReadChar(TzhPacketHeap *p, char* i)
{
	memcpy(i, &p->btBuf[(int)p->wPos], sizeof(char));
	p->wPos+=sizeof(char);
	return sizeof(char);
}

int zhPackHeapWriteShort(TzhPacketHeap *p, short i)
{
	if((int)(p->wPos+sizeof(short))>p->nBufLenght){return 0;}
	memcpy(&p->btBuf[(int)p->wPos], &i, sizeof(short));
	p->wPos+=sizeof(short);
	p->wSize+=sizeof(short);
	return sizeof(short);
}
int zhPackHeapReadShort(TzhPacketHeap *p, short* i)
{	
	memcpy(i, &p->btBuf[(int)p->wPos], sizeof(short));
	p->wPos+=sizeof(short);
	return sizeof(short);
}
int zhPackHeapWriteInt(TzhPacketHeap *p, int i)
{
	if((int)(p->wPos+sizeof(int))>p->nBufLenght){return 0;}
	memcpy(&p->btBuf[(int)p->wPos], &i, sizeof(int));
	p->wPos+=sizeof(int);
	p->wSize+=sizeof(int);
	return sizeof(int);
}
int zhPackHeapReadInt(TzhPacketHeap *p, int* i)
{
	memcpy(i, &p->btBuf[(int)p->wPos], sizeof(int));
	p->wPos+=sizeof(int);
	return sizeof(int);
}
int zhPackHeapWriteLong(TzhPacketHeap *p, long i)
{
	if((int)(p->wPos+sizeof(long))>p->nBufLenght){return 0;}
	memcpy(&p->btBuf[(int)p->wPos], &i, sizeof(long));
	p->wPos+=sizeof(long);
	p->wSize+=sizeof(long);
	return sizeof(long);
}
int zhPackHeapReadLong(TzhPacketHeap *p, long* i)
{
	memcpy(i, &p->btBuf[(int)p->wPos], sizeof(long));
	p->wPos+=sizeof(long);
	return sizeof(long);
}
int zhPackHeapWriteString(TzhPacketHeap *p, char* str)
{
	size_t len=0;
	
	len =strlen(str);

	if((int)(p->wPos+len)>p->nBufLenght){return 0;}

	strcpy((char*)&p->btBuf[(int)p->wPos], str);
	p->btBuf[len + (int)p->wPos] =0;
	len += 1;
	
	p->wPos+=(unsigned short)len;
	p->wSize+=(unsigned short)len;
	return (int)len;
}

int zhPackHeapWriteStringUnicode(TzhPacketHeap *p, char* str)
{
	size_t len=0;

	while(str[len]!=0 || str[len+1]!=0)
	{len+=2;}

	if((int)(p->wPos+len)>p->nBufLenght){return 0;}

	memcpy(&p->btBuf[(int)p->wPos], str,len);
	p->btBuf[(int)p->wPos+len] = 0;
	p->btBuf[(int)p->wPos+len+1] = 0;
	len += 2;

	p->wPos+=(unsigned short)len;
	p->wSize+=(unsigned short)len;
	return (int)len;
}

int zhPackHeapReadString(TzhPacketHeap *p, char* str)
{
	size_t len=0;

	len = strlen((char*)&p->btBuf[(int)p->wPos]);
	strcpy(str, (char*)&p->btBuf[(int)p->wPos]);
	str[len] = 0;
	len += 1;

	p->wPos+=(unsigned short)len;
	return (int)len;
}

int	zhPackHeapReadStringN(TzhPacketHeap *p, char* str ,int strMaxLen)
{
	size_t len=0;
	str[strMaxLen-1]=0;
	strncpy(str, (char*)&p->btBuf[(int)p->wPos],strMaxLen-2);
	len = strlen(str);
	str[len] = 0;
	len += 1;

	p->wPos+=(unsigned short)len;
	return (int)len;
}

int zhPackHeapReadStringUnicode(TzhPacketHeap *p, char* str)
{
	size_t len=0;

	while(p->btBuf[(int)p->wPos+len]!=0 || p->btBuf[(int)p->wPos+len+1]!=0)
	{len+=2;}

	memcpy(str,&p->btBuf[(int)p->wPos], len);
	str[len] = 0;
	str[len+1] = 0;
	len += 2;
	
	p->wPos+=(unsigned short)len;
	return (int)len;
}

int zhPackHeapWriteBinary(TzhPacketHeap *p, char* str ,int len)
{
	if((int)p->wPos+len>p->nBufLenght || len<0){return 0;}
	memcpy(&p->btBuf[(int)p->wPos], str,len);
	p->wPos+=len;
	p->wSize+=len;
	return len;
}
int zhPackHeapReadBinary(TzhPacketHeap *p, char* str ,int len)
{
	if(len<0 || len>p->nBufLenght){return 0;}
	memcpy(str, &p->btBuf[(int)p->wPos],len);
	p->wPos+=len;
	return len;
}
int zhPackHeapWriteBool(TzhPacketHeap *p,bool b)
{
	char a=b?1:0;
	if((int)p->wPos+1>p->nBufLenght){return 0;}
	memcpy(&p->btBuf[(int)p->wPos], &a, 1);
	p->wPos+=1;
	p->wSize+=1;
	return 1;
}
int zhPackHeapReadBool(TzhPacketHeap *p, bool*b)
{
	memcpy(b, &p->btBuf[(int)p->wPos], 1);
	p->wPos+=1;
	return 1;
}


////////////////////////////
int zhPackHeapWriteUnsignedChar(TzhPacketHeap *p,unsigned char i)
{
	if((int)p->wPos+1>p->nBufLenght){return 0;}
	memcpy(&p->btBuf[(int)p->wPos], &i, 1);
	p->wPos+=1;
	p->wSize+=1;
	return 1;
}
int zhPackHeapReadUnsignedChar(TzhPacketHeap *p,unsigned char* i)
{
	memcpy(i, &p->btBuf[(int)p->wPos], sizeof(unsigned char));
	p->wPos+=1;
	return 1;
}

int zhPackHeapWriteUnsignedShort(TzhPacketHeap *p,unsigned short i)
{
	if((int)p->wPos+2>p->nBufLenght){return 0;}
	memcpy(&p->btBuf[(int)p->wPos], &i, 2);
	p->wPos+=2;
	p->wSize+=2;
	return 2;
}
int zhPackHeapReadUnsignedShort(TzhPacketHeap *p,unsigned short* i)
{
	memcpy(i, &p->btBuf[(int)p->wPos], 2);
	p->wPos+=2;
	return 2;
}
int zhPackHeapWriteUnsignedInt(TzhPacketHeap *p,unsigned int i)
{
	if((int)p->wPos+4>p->nBufLenght){return 0;}
	memcpy(&p->btBuf[(int)p->wPos], &i, 4);
	p->wPos+=4;
	p->wSize+=4;
	return 4;
}
int zhPackHeapReadUnsignedInt(TzhPacketHeap *p,unsigned int* i)
{
	memcpy(i, &p->btBuf[(int)p->wPos], 4);
	p->wPos+=4;
	return 4;
}
int zhPackHeapWriteUnsignedLong(TzhPacketHeap *p,unsigned long i)
{
	if((int)p->wPos+8>p->nBufLenght){return 0;}
	memcpy(&p->btBuf[(int)p->wPos], &i, 8);
	p->wPos+=8;
	p->wSize+=8;
	return 8;
}
int zhPackHeapReadUnsignedLong(TzhPacketHeap *p,unsigned long* i)
{
	memcpy(i, &p->btBuf[(int)p->wPos], 8);
	p->wPos+=8;
	return 8;
}
