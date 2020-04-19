#include "main_proc.h"
#include "print_msg.h"
#include "ck_assist.h"
#include "json/cJSON.h"
#include "udp_magr.h"

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

	//--------------------------------------------------------------------------
	PrintDebugMsgEnable(true);
	//--------------------------------------------------------------------------

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
	}

	//��ʼ��
	INIT_CS(&csUSrcData);
	INIT_CS(&csUDstData);
	_udpMagr.setdelegate.set_recv_cb(this,&MainPROC::udp_recvf);
	_udpMagr.init(6699);

	SYS_PRINTF("Load successfully");
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
				itUDst=lstUDstData.erase(itUDst);
		}
		else
		{
				itUDst++;
		}
	}
	UNLOCK_CS(&csUDstData);

	//�������ݵ�ת��
	LOCK_CS(&csUSrcData);
	LOCK_CS(&csUDstData);
	for(itUSrc=lstUSrcData.begin(); itUSrc!=lstUSrcData.end();)
	{
			//ת��
			for(itUDst=lstUDstData.begin(); itUDst!=lstUDstData.end();itUDst++)
			{
				//�ж��Ƿ��Ѿ����Ĵ���Ϣ
				if(0==strcmp(itUDst->flag_string,"all") || 
					0==strcmp(itUDst->flag_string,itUSrc->flag_string))
				{
					_udpMagr.sendto(itUDst->ipv4,itUDst->port,itUSrc->hexbuf,itUSrc->hexlen);
				}
			}
			//�Ƴ�
			itUSrc=lstUSrcData.erase(itUSrc);
	}
	UNLOCK_CS(&csUDstData);
	UNLOCK_CS(&csUSrcData);
	return true;
}

void MainPROC::EndProc()
{
	_udpMagr.destory();
	DELETE_CS(&csUSrcData);
	DELETE_CS(&csUDstData);
}

//--------------------------------------------------
void MainPROC::udp_recvf(char*ip,int port,char* data,int len)
{
	DEBUG_PRINTF("udp_recvf ip=%s, port=%d,int len=%d)",ip,port,len);
	
	data[len]=0x00;
	//������Ϣ���������--
	if(len>6)
	{
			if(0==memcpy(data,"SIGN,",5))
			{
						if(0x00!=(data+6))
						{
									if(lstUDstData.size() < MAX_LIST_UDST_DATA_COUNT)
									{
												LOCK_CS(&csUDstData);
												//�жϼ�¼�Ƿ��Ѿ�����
												for(itUDst=lstUDstData.begin(); itUDst!=lstUDstData.end();itUDst++)
												{
															//�ж��Ƿ��Ѿ����Ĵ���Ϣ
															if(0==strcmp(itUDst->flag_string,"all") || 
																0==strcmp(itUDst->flag_string,itUSrc->flag_string))
															{
																	itUDst->activeTime=zhPlatGetTime();
																	break;
															}
												}

												//���������ݵ����λ����Ϣ
												TagUserDstData usda;
												strncpy(usda.flag_string,data+6,126);
												strncpy(usda.ipv4,ip,18);
												usda.port=port;
												usda.activeTime=zhPlatGetTime();
												lstUDstData.push_back(usda);
												UNLOCK_CS(&csUDstData);
									}
									else
									{
											DEBUG_PRINTF("lstUDstData too many record");
									}
						}
			}
			else if(0==memcpy(data,"DATA,",5))
			{
					if(lstUSrcData.size() < MAX_LIST_UDST_DATA_COUNT)
					{
							if(0x00!=(data+6))
							{
									LOCK_CS(&csUSrcData);
									TagUserSrcData usda;
									char *pflag=data+6;
									int pbuflen=len-strlen(pflag)+6;
									char*pbuf=strchr(pflag,',');
									if(pbuf)
									{
											*pbuf=0x00;
											pbuf++;
											strncpy(usda.flag_string,pflag,126);
											memcpy(usda.hexbuf,pbuf,pbuflen);
											lstUSrcData.push_back(usda);
											UNLOCK_CS(&csUSrcData);
									}
									else
									{
											DEBUG_PRINTF("protocol \"DATA:\" format error");
									}
							}
					}
					else
					{
							DEBUG_PRINTF("lstUSrcData too many record");
					}
			}
			else
			{
					DEBUG_PRINTF("unknow protocol format");
			}
	}
	else
	{
			DEBUG_PRINTF("udp data len too short");
	}
}
