/*
  gcdef.h:the transmission manage
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



#ifndef __gcdef_h__
#include "platform.h"

#pragma pack(1)
 
/*
 *
 *Begin---------->Network layer protocol in GCHPacket.h module docking
 *
 *Begin---------->�����Խ�Э����GCHPacket.hģ����
*/

#define ZH_NET_FRAME_FLAG	0x0A	//zhNet�����֡�̶�ֵ

typedef struct _TzhPackFrameHeader
{
/*
 *Packet type, EzhPackType structural break
 *
 *��������,EzhPackType�ṹ��
*/
	unsigned char   yFrameFlag;	//�̶� ZH_NET_FRAME_FLAG
	unsigned char	wDataLenL1;   //����İ���ĳ���
	unsigned char	wDataLenL2;   //����İ���ĳ��ȵڶ��ֽ�
	unsigned char	wCRC16L1;     //�����CRCУ��,��������Ϊ0ʱCRCУ��ֵΪ0
	unsigned char	wCRC16L2;     //�����CRCУ��,��������Ϊ0ʱCRCУ��ֵΪ0,�ڶ��ֽ�
}TzhPackFrameHeader;

/*
 *Begin---------->In the GCHNetUtil.h definition
 *
 *Begin---------->��GCHNetUtil.h����
 */
 
/*
 *Each time the maximum length, not including package head
 *
 *ÿ�η�������󳤶ȣ���������ͷ
*/
#define ZH_NET_PACKET_BODY_LENGTH			1024
#define ZH_NET_TCP_CACHE_LENGTH				1024*15
			
//End---------->

#pragma pack()

#define __gcdef_h__
#endif
