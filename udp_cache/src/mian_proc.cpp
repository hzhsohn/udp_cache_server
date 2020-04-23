#include "main_proc.h"
#include "print_msg.h"
#include "ck_assist.h"
#include "json/cJSON.h"
#include "udp_magr.h"
#include "assist_func.h"
#include "assist_func2.h"



//����ת��Դ�������Ľ�������,10������¼,Լռ�ڴ�190M
#define MAX_LIST_USRC_DATA_COUNT			100000
//���ݽ���Դ�������Ľ�������,1������¼
#define MAX_LIST_UDST_DATA_COUNT			10000
//��ʱ������û����ͱ��Ƴ���Դ
#define MAX_USER_ACTIVE_TIMEOUT				30000



bool MainPROC::InitProc(int argc,char *argv[])
{
	int isSetWorkPath=0;
	int iParam=0;

	//��ȡ�Ƿ�Ϊ��Ŀ����ģʽ�Ĳ���
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
			//�򿪵�����Ϣ
			PrintDebugMsgEnable(true);
		}
	}

	//��ʼ��
	INIT_CS(&csTrans);
	INIT_CS(&csUDstData);
	_udpMagr.setdelegate.set_recv_cb(this,&MainPROC::udp_recvf);
	if(_udpMagr.init(6699))
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
	//������ڵ�Ŀ��Դ
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
	UNLOCK_CS(&csUDstData);

	//�������ݵ�ת��
	LOCK_CS(&csTrans);
	LOCK_CS(&csUDstData);
	for(itTrans=lstTransData.begin(); itTrans!=lstTransData.end();)
	{
			DEBUG_PRINTF("lstTransData process flag=%s, len=%d, sign count=%d",
					itTrans->flag_string,itTrans->hexlen,lstUDstData.size());
			//ת��
			for(itUDst=lstUDstData.begin(); itUDst!=lstUDstData.end();itUDst++)
			{
				//�ж��Ƿ��Ѿ����Ĵ���Ϣ
				if(0==strcmp(itUDst->flag_string,"all") || 
					0==strcmp(itUDst->flag_string,itTrans->flag_string))
				{
					_udpMagr.sendto(itUDst->ipv4,itUDst->port,itTrans->hexbuf,itTrans->hexlen);
				}
			}
			//�Ƴ�
			itTrans=lstTransData.erase(itTrans);
	}
	UNLOCK_CS(&csUDstData);
	UNLOCK_CS(&csTrans);
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
	
	//������Ϣ���������--
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
								//�жϼ�¼�Ƿ��Ѿ�����
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
											//�ж��Ƿ��Ѿ����Ĵ���Ϣ
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
											//���������ݵ����λ����Ϣ
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
			else if(0==memcmp(data,"DATA,",5))
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
												strncpy(usda.flag_string,pflag,126);
												memcpy(usda.hexbuf,pbuf,pbuflen);
												usda.hexlen=pbuflen;
												lstTransData.push_back(usda);
											}
											else
											{
												DEBUG_PRINTF("protocol \"DATA,\" lost content");
											}
									}
									else
									{
											DEBUG_PRINTF("protocol \"DATA,\" format error");
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
