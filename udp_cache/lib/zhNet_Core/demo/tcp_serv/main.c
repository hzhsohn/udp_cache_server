#include "session.h"

TzhNetListen g_listern;
TzhNetSession clientUser;

void zhRecv(TzhNetSession *sion,void* info,char*szBuf,int nLen);
void zhDisconnect(TzhNetSession*sion,void* info);
void zhError(TzhNetSession* sion,void* info,EzhNetError err);

int main(int argc,char *argv[])
{
	unsigned char frame[ZH_NET_PACKET_BODY_LENGTH];
	EzhNetError err;
	int ret;
	TzhNetSession tmpUser;
	
	//��ʼ������,����ͨѶ
	ret=zhSionStartup(&g_listern,2323,true);
	if(ret)
	{
		PRINTF("Startup Server ok..!!");
		PRINTF("BindPort=%d",2323);
		while(true)
		{
				zhPlatSleep(1);
		
				//
				if(zhSionAccept(&g_listern,&tmpUser))
				{
					clientUser=tmpUser;
				}

				//�������ݴ���-------begin
				if(false==zhSionCacheData(&clientUser,&err))
				{
					zhError(&clientUser,&clientUser.pInfo,err);
				}
				while(1)
				{
					ret=zhSionReadData(&clientUser,frame,&err);
					if(0==ret)
					{ break; }
					else if(ret>0)
					{
						//����frame����
						zhRecv(&clientUser,
							clientUser.pInfo,
							frame,
							ret);
					}
					else
					{
						zhError(&clientUser, clientUser.pInfo, err);
					}
				}
				switch(zhSionStateThread(&clientUser))
				{
						//���ӳɹ�
					case ezhNetEventConnected:
						break;
						//����ʧ��
					case ezhNetEventConnectTimeout:
						break;
						//�Ͽ�����
					case ezhNetEventDisconnect:
						zhDisconnect(&clientUser,
							clientUser.pInfo);
						break;
						//û���¼�
					case ezhNetNoEvent:
						break;
				}
				//------------------end
		}
	}
	return 0;
}

void zhRecv(TzhNetSession *sion,void* info,unsigned char*szBuf,int nLen)
{
	TzhPacket pack;
	int idid;
	char sstr[550];

	zhPackReadInit(&pack,(BYTE*)szBuf,nLen);
	zhPackReadInt(&pack,&idid);
	zhPackReadString(&pack,sstr);

	PRINTF("zhRecv  nLen=%d  id=%d str=%s",nLen,idid,sstr);

	zhPackWriteInit(&pack);
	zhPackWriteString(&pack,"���������͹�ȥ����Ϣ..");	
	zhSionSendPacket(sion,&pack);
}

void zhDisconnect(TzhNetSession*sion,void* info)
{
	PRINTF("Disconnected..!!  Socket=%d ",sion->s);
}

void zhError(TzhNetSession* sion,void* info,EzhNetError err)
{
	switch (err)
	{
		//�յ����ݰ�����
		case ezhNetErrorPacketInvalid:
		break;
		//���������
		case ezhNetErrorCacheOverflow:
		break;
		//CRCУ�����
		case ezhNetErrorCRC16:
		break;
	}

}
