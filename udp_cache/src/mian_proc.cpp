#include "main_proc.h"
#include "print_msg.h"
#include "ck_assist.h"
#include "json/cJSON.h"
#include "udp_magr.h"
#include "assist_func.h"
#include "assist_func2.h"



//数据转发源最大链表的接收数量,10万条记录,约占内存190M
#define MAX_LIST_TRANS_DATA_COUNT			400000
//数据接收源最大链表的接收数量,1万条记录
#define MAX_LIST_UDST_DATA_COUNT			200000
//超时多少秒没激活就被移除接源
#define MAX_USER_ACTIVE_TIMEOUT				30000

//UDP发送的缓存
char g_udpSendCache[1620];
int g_udpSendCacheLen;

bool MainPROC::InitProc(int argc,char *argv[])
{
	int isSetWorkPath=0;
	int iParam=0;
	int nBindPort=6699;

	//获取是否为项目调试模式的参数
	for (iParam=0; iParam < argc; iParam++)
	{
        printf("Argument %d is %s\n", iParam, argv[iParam]);
	}
		printf("------------------------------------------------\r\n");
    for (iParam=0; iParam < argc; iParam++)
	{
		if(0==strcmp(argv[iParam],"-?") ||0==strcmp(argv[iParam],"-help") ||
		   0==strcmp(argv[iParam],"?") ||0==strcmp(argv[iParam],"help"))
		{
			printf("used parameter:\r\n");
			printf("\r\n");

			exit(0);
		}
		else if(0==strcmp(argv[iParam],"-debug"))
		{
			//打开调试信息
			PrintDebugMsgEnable(true);
		}
		else if(0==strcmp(argv[iParam],"-port"))
		{
			if(iParam<argc-1)
			{
				int np=atoi(argv[iParam+1]);
				if(np>0)
				{
					nBindPort=np;
				}
                else
                {
                   SYS_PRINTF("parameter -port error value=%s",argv[iParam+1]);
                }
			}
			else
			{
                SYS_PRINTF("parameter -port no value");
            }
		}
	}

	//初始化
	INIT_CS(&csTrans);
	INIT_CS(&csUDstData);
	_udpMagr.setdelegate.set_recv_cb(this,&MainPROC::udp_recvf);
	if(_udpMagr.init(nBindPort))
	{
		SYS_PRINTF("Load successfully");
	}
	else
	{
		SYS_PRINTF("Service startup fail.");
	}
	return true;
}

bool MainPROC::RunProc()
{
	zhPlatSleep(1);
	//处理过期的目标源
	LOCK_CS(&csUDstData);
	for(itUDst=lstUDstData.begin(); itUDst!=lstUDstData.end();)
	{
		time_t tmpT=zhPlatGetTime();
		if(tmpT - itUDst->activeTime > MAX_USER_ACTIVE_TIMEOUT)
		{
			DEBUG_PRINTF("lstUDstData remove ip=%s, port=%d,sign_flag=%s",itUDst->ipv4,itUDst->port,itUDst->flag_string);
			itUDst=lstUDstData.erase(itUDst);
		}
		else
		{
			itUDst++;
		}
	}

	//处理数据的转发
	LOCK_CS(&csTrans);
	for(itTrans=lstTransData.begin(); itTrans!=lstTransData.end();)
	{
			DEBUG_PRINTF("lstTransData process flag=%s, len=%d, sign count=%d",
					itTrans->flag_string,itTrans->hexlen,lstUDstData.size());
			//转发
			for(itUDst=lstUDstData.begin(); itUDst!=lstUDstData.end();itUDst++)
			{
				//判断是否已经订阅此消息
				if(0==strcmp(itUDst->flag_string,"all") || 
					0==strcmp(itUDst->flag_string,itTrans->flag_string))
				{
					snprintf(g_udpSendCache,sizeof(g_udpSendCache),"RECV,%s,\0",itTrans->flag_string);
					g_udpSendCacheLen=strlen(g_udpSendCache);
					memcpy(g_udpSendCache+g_udpSendCacheLen,itTrans->hexbuf,itTrans->hexlen);
					g_udpSendCacheLen+=itTrans->hexlen;
					_udpMagr.sendto(itUDst->ipv4,itUDst->port,g_udpSendCache,g_udpSendCacheLen);
				}
			}
			//移除
			itTrans=lstTransData.erase(itTrans);
	}

	UNLOCK_CS(&csTrans);
	UNLOCK_CS(&csUDstData);
	return true;
}

void MainPROC::EndProc()
{
	_udpMagr.destory();
	DELETE_CS(&csTrans);
	DELETE_CS(&csUDstData);
}

//--------------------------------------------------
void MainPROC::udp_recvf(char*ip,int port,char* data,int len)
{
	data[len]=0x00;
	
	//区分信息存入的链表--
	if(len>5)
	{
			if(0==memcmp(data,"SIGN,",5) && 0x00!=(data+5))
			{
					if(lstUDstData.size() < MAX_LIST_UDST_DATA_COUNT)
					{
							char* pdataStr=data+5;
							vector<string> strAry;
							vector<string>::iterator itStr;
							stringSplit(pdataStr,"/",strAry);
							if(strAry.size()>0)
							{
								//
								LOCK_CS(&csUDstData);
								//判断记录是否已经存在
								for(itStr=strAry.begin();itStr!=strAry.end(); itStr++)
								{
									BOOL isNew=TRUE;
									char sfbuf[300]={0};
									strncpy(sfbuf,itStr->c_str(),298);
									trim(sfbuf);
									if(0x00!=sfbuf[0])
									{
										for(itUDst=lstUDstData.begin(); itUDst!=lstUDstData.end();itUDst++)
										{
											//判断是否已经订阅此消息
											if( itUDst->port==port &&
												0==strcmp(itUDst->ipv4,ip) &&
												0==strcmp(itUDst->flag_string,sfbuf))
											{
													itUDst->activeTime=zhPlatGetTime();
													isNew=FALSE;
													DEBUG_PRINTF("sign \"%s\" active time",sfbuf);
													break;
											}
										}
															
										if(isNew)
										{
											//新增将数据到达的位置信息
											TagUserDstData usda;
											strncpy(usda.flag_string,sfbuf,94);
											strncpy(usda.ipv4,ip,18);
											usda.port=port;
											usda.activeTime=zhPlatGetTime();
											lstUDstData.push_back(usda);
											DEBUG_PRINTF("sign add \"%s\" , lstUDstData.count=%d",sfbuf,lstUDstData.size());
										}
									}
								}
													
								UNLOCK_CS(&csUDstData);
							}
					}
					else
					{
							DEBUG_PRINTF("lstUDstData too many record");
					}
			}
			else if(0==memcmp(data,"SEND,",5))
			{
					if(lstTransData.size() < MAX_LIST_UDST_DATA_COUNT)
					{							
							char* pdataStr=data+5;
							if(0x00!=pdataStr)
							{
									LOCK_CS(&csTrans);
									TagUserSrcData usda;
									char *pflag=pdataStr;
									char*pbuf=strchr(pflag,',');
									if(pbuf)
									{
											*pbuf=0x00;
											pbuf++;
											if(*pbuf)
											{
												int pbuflen=len-(strlen(pflag)+6);
												if(pbuflen<sizeof(usda.hexbuf))
												{
													strncpy(usda.flag_string,pflag,126);
													memcpy(usda.hexbuf,pbuf,pbuflen);
													usda.hexlen=pbuflen;
													lstTransData.push_back(usda);
												}
												else
												{
													DEBUG_PRINTF("protocol \"SEND,\" content length too long.max=%d",sizeof(usda.hexbuf)-1);
												}
											}
											else
											{
												DEBUG_PRINTF("protocol \"SEND,\" lost content");
											}
									}
									else
									{
											DEBUG_PRINTF("protocol \"SEND,\" format error");
									}
									UNLOCK_CS(&csTrans);
							}
					}
					else
					{
							DEBUG_PRINTF("lstTransData too many record");
					}
			}
			else
			{
					DEBUG_PRINTF("-----------------------------------------------");
					DEBUG_PRINTF("unknow protocol format");
					DEBUG_PRINTF("ip=%s, port=%d,len=%d,data=%s",ip,port,len,data);
					DEBUG_PRINTF("-----------------------------------------------");
			}
	}
	else
	{
			DEBUG_PRINTF("udp data len too short");
	}
}
