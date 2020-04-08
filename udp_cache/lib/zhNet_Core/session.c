#include "session.h"

bool zhSionStartup(TzhNetListen* lisn,unsigned short wBindPort,bool isVariableFlowEncrypt)
{
	//srand(zhPlatGetTime());
	if(!zhSockInit(&lisn->s,ZH_SOCK_TCP)){ZH_NET_DEBUG_PRINTF("Init Socket Fail..!!");return false;}
    if(!zhSockSetNonBlocking(lisn->s,true)){ZH_NET_DEBUG_PRINTF("SetNonBlocking Fail..!!");return false;}
	if(!zhSockSetReuseAddr(lisn->s,true)){ZH_NET_DEBUG_PRINTF("Reuse Addr Fail..!!");return false;}
	if(!zhSockBindAddr(lisn->s,NULL,wBindPort)){ZH_NET_DEBUG_PRINTF("Bind Fail..!!");return false;}
	if(!zhSockListen(lisn->s))return false;

	lisn->isStartupVariableFlowEncrypt=isVariableFlowEncrypt;

	return true;
}

bool zhSionInit(TzhNetSession *sion,unsigned short wBindPort)
{
	//srand(zhPlatGetTime());
	if(!zhSockInit(&sion->s,ZH_SOCK_TCP)){return false;}
    if(!zhSockSetNonBlocking(sion->s,true)){ZH_NET_DEBUG_PRINTF("SetNonBlocking Fail..!!");return false;}
	
	sion->pInfo=NULL;
	sion->cState=ezhNetStateInitizal;
	sion->isStartupVariableFlowEncrypt=false;
	memset(sion->nEncryptKey,0,4);
	sion->isAlreadyGetEncryptSeed=false;

	sion->tagPack.btCache=NULL;
	sion->tagPack.wNetPackPos=0;

	if(wBindPort>0)
	{
        if(!zhSockSetReuseAddr(sion->s,true)){ZH_NET_DEBUG_PRINTF("Reuse Addr Fail..!!");return false;}
        if(!zhSockBindAddr(sion->s,NULL,wBindPort)){return false;}
    }
	return true;
}

void zhSionSetInfo(TzhNetSession *sion,void* pInfo)
{
	sion->pInfo=pInfo;
}

bool zhSionConnect(TzhNetSession *sion,char*szIp,unsigned short wRemotoPort)
{
	if(false==zhSockConnect(sion->s,szIp,wRemotoPort,0)){
		return false;
	}
	sion->cState=ezhNetStateWaitSeed;
	sion->dwStartTime=zhPlatGetTime();
	
	sion->tagPack.bNetPackRecvBuf=false;
	sion->tagPack.btCache=(unsigned char*)malloc(ZH_NET_PACKET_SIZE);
	sion->tagPack.wNetPackPos=0;
	sion->isAlreadyGetEncryptSeed=false;
	return true;
}

void zhSionSetBigSockCache(TzhNetSession *sion,int buf_size)
{
	//如果连接数多跟大量收发数据的话就启用这项  zhSionConnect   zhSionAccept 的函数后面执行
	zhSockSetSendBufferSize(sion->s,buf_size);//设置发送缓冲区
	zhSockSetRecvBufferSize(sion->s,buf_size);//设置接收缓冲区
}

bool zhSionAccept(TzhNetListen* lisn,TzhNetSession *sion)
{
    sion->s=zhSockAccept(lisn->s);
	if(sion->s)
	{
		sion->dwStartTime=zhPlatGetTime();		
		sion->pInfo=NULL;

		sion->tagPack.bNetPackRecvBuf=false;
		sion->tagPack.btCache=(unsigned char*)malloc(ZH_NET_PACKET_SIZE);
		sion->tagPack.wNetPackPos=0;
		sion->cState=ezhNetStateInitizal;

		//加密信息
		sion->isStartupVariableFlowEncrypt=lisn->isStartupVariableFlowEncrypt;
		memset(sion->nEncryptKey,0,4);
		_zhSionSendKeyEncrypt(sion);
		sion->isAlreadyGetEncryptSeed=true;
		return true;
	}
	return false;
}

bool zhSionSafeClose(TzhNetSession *sion)
{
    if(ezhNetStateZero!=sion->cState)
	{
        sion->cState=ezhNetStateDead;
    }
	if(sion->tagPack.btCache)
	{
		free(sion->tagPack.btCache);
		sion->tagPack.btCache=NULL;
	}
	if(sion->s)
	{
		zhSockClose(sion->s);
		sion->s=0;
	}
	//
	sion->tagPack.wNetPackPos=0;
	sion->tagPack.bNetPackRecvBuf=false;
	return true;
}
bool zhSionClose(TzhNetSession *sion)
{
    //关闭连接
	if(sion->s)
	{
		//关闭双向通信
		zhSockShutdown(sion->s,ezhNetShutDownRDWR);
		zhSockClose(sion->s);
		sion->s=0;
	}
	//
	sion->tagPack.wNetPackPos=0;
	sion->tagPack.bNetPackRecvBuf=false;
	if(sion->tagPack.btCache)
	{
		free(sion->tagPack.btCache);
		sion->tagPack.btCache=NULL;
	}
	sion->cState=ezhNetStateZero;
	return true;
}
int zhSionSend(TzhNetSession *sion,char* szPack,int nLen)
{
	int nSendLen=0;
	TzhPackFrameHeader pktHeader={0};
	unsigned char* sendBuf=NULL;
	int ret;
	unsigned short tmp=0;

	if(ezhNetStateDead==sion->cState || ezhNetStateZero==sion->cState)
	{
		return -1;
	}

	nSendLen=sizeof(TzhPackFrameHeader)+nLen;
	sendBuf=(unsigned char*)malloc(nSendLen);

	//++++++++++++++++++
	//包头数据
	pktHeader.yFrameFlag=ZH_NET_FRAME_FLAG;
	pktHeader.wDataLenL1=nLen&0xFF;
	pktHeader.wDataLenL2=nLen>>8;
	tmp=zhPlatCRC16((unsigned char*)szPack,nLen);
	pktHeader.wCRC16L1=tmp&0xFF;
	pktHeader.wCRC16L2=tmp>>8;

	//-----------------
	memcpy(sendBuf,&pktHeader,sizeof(TzhPackFrameHeader));
	memcpy(&sendBuf[sizeof(TzhPackFrameHeader)],szPack,nLen);

	//zhPlatPrint16(4,&sion->nEncryptKey);
	//Encrypt Key
	zhNetEncrypt(sion->isStartupVariableFlowEncrypt,(char*)sendBuf,nSendLen,sion->nEncryptKey);
	//添加到发送列表
	ret=zhSockSend(sion->s,(char*)sendBuf,nSendLen);
	
	free(sendBuf);
	sendBuf=NULL;
	return ret;
}

int zhSionSendPacket(TzhNetSession *sion,TzhPacket*pack)
{
	return zhSionSend(sion,(char*)pack->btBuf,pack->wSize);
}

bool zhSionCacheData(TzhNetSession *sion,EzhNetError* err)
{	
	char		szTmpBuf[ZH_NET_PACKET_BODY_LENGTH];
	int			nBufSize;
	
	if(!sion->s)
	{
		*err=ezhNetErrorSocket;
		return false;
	}

	nBufSize=zhSockRecv(sion->s,szTmpBuf,ZH_NET_PACKET_BODY_LENGTH);

	if(nBufSize!=SOCKET_ERROR)
	{
		if(nBufSize>0)
		{
			//最后一次接收数据包的时间
			sion->tagPack.lastRecvBufTime=zhPlatGetTime();
			//如果缓存区积压太大的话直接断开SOCKET
			if(sion->tagPack.wNetPackPos+nBufSize>=ZH_NET_PACKET_SIZE)
			{
				ZH_NET_DEBUG_PRINTF("Cache Waring Socket=%d , wNetPackPos=%d , nBufSize=%d",sion->s,sion->tagPack.wNetPackPos,nBufSize);
				sion->tagPack.wNetPackPos=0;
				*err=ezhNetErrorCacheOverflow;
				return false;
			}
			else
			{
				//客户端获取种子钥匙
				if(false==sion->isAlreadyGetEncryptSeed)
				{
					memcpy(&sion->tagPack.btCache[sion->tagPack.wNetPackPos],szTmpBuf,nBufSize);
					sion->tagPack.wNetPackPos+=nBufSize;

					if(sion->tagPack.wNetPackPos>=4)//4个字节加密信息
					{
							sion->nEncryptKey[0]=szTmpBuf[0];
							sion->nEncryptKey[1]=szTmpBuf[1];
							sion->nEncryptKey[2]=szTmpBuf[2];
							sion->nEncryptKey[3]=szTmpBuf[3];
							//zhPlatPrint16(4,&sion->nEncryptKey);
							if(0==sion->nEncryptKey)//加密信息不0为即是变流加密
							{
								sion->isStartupVariableFlowEncrypt=false;
							}
							else{
								sion->isStartupVariableFlowEncrypt=true;
							}
							ZH_NET_DEBUG_PRINTF("Get Sock=%d, nEncryptKey=%02x %02x %02x %02x",
									sion->s,sion->nEncryptKey[0],sion->nEncryptKey[1],sion->nEncryptKey[2],sion->nEncryptKey[3]);
							sion->cState=ezhNetStateConnecting;
				
							//剩余的全扔到帧缓冲区里
							sion->tagPack.wNetPackPos-=4;//4个字节验证
							if(sion->tagPack.wNetPackPos>0)
							{
								memmove(&sion->tagPack.btCache[sion->tagPack.wNetPackPos],&szTmpBuf[4],sion->tagPack.wNetPackPos);
							}
							sion->isAlreadyGetEncryptSeed=true;
					}
				}
				else
				{
					//zhPlatPrint16(4,&sion->nEncryptKey);
					//还原加密数据
					zhNetDecrypt(sion->isStartupVariableFlowEncrypt,szTmpBuf,nBufSize,sion->nEncryptKey);			
					//
					memcpy(&sion->tagPack.btCache[sion->tagPack.wNetPackPos],szTmpBuf,nBufSize);
					sion->tagPack.wNetPackPos+=nBufSize;
				}
			}
		}
	}
	else
	{
		if(sion->cState!=ezhNetStateZero && sion->cState!=ezhNetStateWaitSeed)
		{sion->cState=ezhNetStateDead;}
	}

	*err=ezhNetNoError;
	return true;
}

int zhSionReadData(TzhNetSession *sion,unsigned char *frame,int frame_len,EzhNetError* err)
{
		int nRet;
		int nTmpPos1,nTmpPos2;
		unsigned long ltmpTime;
		
		nRet=0;

		//判断数据包接收超时判断
		if(sion->tagPack.wNetPackPos>0)
		{
			ltmpTime=zhPlatGetTime();
			if(ltmpTime - sion->tagPack.lastRecvBufTime > ZH_NET_RECV_OVERTIME)
			{
				sion->tagPack.lastRecvBufTime=ltmpTime;
				//清空所有缓存数据
				sion->tagPack.wNetPackPos=0;
				sion->tagPack.btCache[0]=0x00;
				//返回错误
				*err=ezhNetErrorRecvOvertime;
			}
		}

		if(sion->tagPack.wNetPackPos>=sizeof(TzhPackFrameHeader))
		{
			TzhPackFrameHeader packetHeader;
			unsigned short wDataLen=0,wCRC16=0;
			memcpy(&packetHeader,sion->tagPack.btCache,sizeof(TzhPackFrameHeader));
			//
			wDataLen=packetHeader.wDataLenL1|packetHeader.wDataLenL2<<8;
			wCRC16=packetHeader.wCRC16L1|packetHeader.wCRC16L2<<8;
			//++++++++++++++++++++++++处理整个包头+++++++++++++++++++++++++++++++++++++++++
			if(false==sion->tagPack.bNetPackRecvBuf)
			{
				//错误包断开
				if(packetHeader.yFrameFlag !=ZH_NET_FRAME_FLAG)
				{
					int findPos;
					unsigned char*pHeader;
					
					//寻找包开始的字节
					pHeader=(unsigned char*)&packetHeader;
					findPos=sizeof(packetHeader.yFrameFlag);
					while(findPos<sion->tagPack.wNetPackPos)
					{
						if(ZH_NET_FRAME_FLAG==pHeader[findPos])
						{break;} 
						findPos++;
					}
					sion->tagPack.wNetPackPos-=findPos;
					if(sion->tagPack.wNetPackPos>0)
					{
						memmove(sion->tagPack.btCache,&sion->tagPack.btCache[findPos],sion->tagPack.wNetPackPos);
					}

					ZH_NET_DEBUG_PRINTF("Packet Error->Socket=%d , wDataLen=%d",
							sion->s,wDataLen);
					
					*err=ezhNetErrorPacketInvalid;
					return -1;
				}
				else
				{
					sion->tagPack.bNetPackRecvBuf=true;
				}
			}
			//++++++++++++++++++++++++处理具有帧头的数据流+++++++++++++++++++++++++++++++++++++++++
			if(true==sion->tagPack.bNetPackRecvBuf)
			{
				unsigned short wCRCTmp;
				//++++++++++++++++++++++++处理整个完整包+++++++++++++++++++++++++++++++++++++++++
    			nTmpPos1=sizeof(TzhPackFrameHeader)+wDataLen;
    			if(sion->tagPack.wNetPackPos >=nTmpPos1)
    			{					
					//数据包处理----------------------
					wCRCTmp=zhPlatCRC16(&sion->tagPack.btCache[sizeof(TzhPackFrameHeader)],
											wDataLen);
					if(wCRCTmp==wCRC16)
					{
							if(wDataLen <= frame_len)
							{
								memcpy(frame,&sion->tagPack.btCache[sizeof(TzhPackFrameHeader)],wDataLen);
								nRet=wDataLen;
								//更新接收时间
								sion->tagPack.lastRecvBufTime=zhPlatGetTime();
							}
							else
							{
								sion->tagPack.wNetPackPos=0; //复位所有接收过的数据
								*err=ezhNetErrorCacheNotEnough;
								return -1;
							}
					}
					else
					{
						int findPos;
						unsigned char*pPacket;
						
						//寻找包开始的字节
						pPacket=sion->tagPack.btCache;
						findPos=sizeof(packetHeader.yFrameFlag);
						while(findPos<sion->tagPack.wNetPackPos)
						{
							if(ZH_NET_FRAME_FLAG==pPacket[findPos])
							{break;}
							findPos++;
						}
						sion->tagPack.wNetPackPos-=findPos;
						if(sion->tagPack.wNetPackPos>0)
						{
							memmove(sion->tagPack.btCache,&sion->tagPack.btCache[findPos],sion->tagPack.wNetPackPos);
						}

						ZH_NET_DEBUG_PRINTF("CRC16 error wCRCTmp=%d , Frame->header.wCRC16=%d",wCRCTmp,wCRC16);
						//CRC校验出错
						*err=ezhNetErrorCRC16;

						return -1;
					}
					//----------------------------------

    				nTmpPos2=sion->tagPack.wNetPackPos-nTmpPos1;
					if(nTmpPos2>=0)
					{
						if(nTmpPos2>0)
						{memmove(sion->tagPack.btCache,&sion->tagPack.btCache[nTmpPos1],nTmpPos2);}
    					sion->tagPack.wNetPackPos=nTmpPos2;
					}
    				sion->tagPack.bNetPackRecvBuf=false;
    			}
				else
				{
					//跳出循环if(sion->tagPack.wNetPackPos >=nTmpPos)
					return 0;
				}
			}
			else
			{
				//跳出循环if(true==sion->tagPack.bNetPackRecvBuf)
				return 0;
			}
		}
		else
		{
			//跳出循环if(sion->tagPack.wNetPackPos>=sizeof(TzhPackFrameHeader))
			return 0;
		}

		*err=ezhNetNoError;
		
		return nRet;
}

EzhNetEvent zhSionStateThread(TzhNetSession*sion)
{
	//--------------------------------------------
	//状态处理
	switch(sion->cState)
	{
	case ezhNetStateWaitSeed:
		{
			//超时提示
			unsigned long dwTmp;
			unsigned long nTmp;
			dwTmp=zhPlatGetTime();
			nTmp=dwTmp-sion->dwStartTime;
			if(zhSockHasExcept(sion->s,0,0) || nTmp>ZH_NET_CONNECT_TIMEOUT)
			{
				sion->cState=ezhNetStateDead;
				return ezhNetEventConnectTimeout;
			}
		}
		break;
	case ezhNetStateConnecting:
			sion->cState=ezhNetStateConnected;
			return ezhNetEventConnected;
		break;
	case ezhNetStateConnected:

		break;
	case ezhNetStateDead:
		{
			//关闭连接
			if(sion->s)
			{
				//关闭双向通信
				zhSockShutdown(sion->s,ezhNetShutDownRDWR);
				zhSockClose(sion->s);
				sion->s=0;
			}
			//
			sion->tagPack.wNetPackPos=0;
			sion->tagPack.bNetPackRecvBuf=false;
			if(sion->tagPack.btCache)
			{
				free(sion->tagPack.btCache);
				sion->tagPack.btCache=NULL;
			}
			sion->cState=ezhNetStateZero;
			return ezhNetEventDisconnect;
		}
		break;
	case ezhNetStateZero:
			//ZH_NET_DEBUG_PRINTF("ezhNetStateDead  Socket=%d",sion->s);
		break;
	}
	return ezhNetNoEvent;
}

bool zhSionLocalAddr(TzhNetSession *sion,char *addr,unsigned short *port,unsigned int *ip)
{
	return zhSockGetLocalAddr(sion->s,addr,port,(unsigned long*)ip);
}

bool zhSionRemoteAddr(TzhNetSession *sion,char *addr,unsigned short *port,unsigned int *ip)
{
	return zhSockGetRemoteAddr(sion->s,addr,port,(unsigned long*)ip);
}

//发送种子钥匙
void _zhSionSendKeyEncrypt(TzhNetSession *sion)
{
	int nEncryptKey=0;

	//固定发送4个字节	
	if(sion->isStartupVariableFlowEncrypt)
	{
		nEncryptKey=zhNetGetRandEncryptKey();		
	}
	else
	{
		nEncryptKey=0;
	}
	//zhPlatPrint16(4,&nEncryptKey);
	sion->nEncryptKey[0]=nEncryptKey&0xFF;
	sion->nEncryptKey[1]=nEncryptKey>>8&0xFF;
	sion->nEncryptKey[2]=nEncryptKey>>16&0xFF;
	sion->nEncryptKey[3]=nEncryptKey>>24&0xFF;

	if(zhSockSend(sion->s,(char*)sion->nEncryptKey,4))
	{sion->cState=ezhNetStateConnected;}

	ZH_NET_DEBUG_PRINTF("SetKey Sock=%d, nEncryptKey=%02x %02x %02x %02x",
		sion->s,sion->nEncryptKey[0],sion->nEncryptKey[1],sion->nEncryptKey[2],sion->nEncryptKey[3]);
}
