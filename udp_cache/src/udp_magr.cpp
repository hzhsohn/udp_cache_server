#include "udp_magr.h"
#include "print_msg.h"
#include "ck_assist.h"
#include "json/cJSON.h"


UDP_MAGR::UDP_MAGR()
{
	isRuning=FALSE;
}

UDP_MAGR::~UDP_MAGR()
{
}

void UDP_MAGR::init(int port)
{

	//socket1
	zhSockInit(&_udpSocket,ZH_SOCK_UDP);
	if(zhSockSetReuseAddr(_udpSocket,true))
	{
		SYS_PRINTF("udp port : %d",port);
		zhSockBindAddr(_udpSocket,NULL,port); 
	}

	//������Ĭ��1M 
	zhSockSetSendBufferSize(_udpSocket,1024*1024);
    zhSockSetRecvBufferSize(_udpSocket,1024*1024);

	isRuning=TRUE;
	CREATE_THREAD(&UDP_MAGR::udpThread,this);

}


void UDP_MAGR::destory()
{
	isRuning=FALSE;
	zhPlatSleep(100);
}


//�����Խ�������
int UDP_MAGR::blockingRecvfrom(SOCKET s,char *buf, int buf_len, struct sockaddr_in *addr ,int *addrlen)
{
	int ret;
	if (!zhSockCanRead(s,2,0)) return 0;

	ret = recvfrom(s,buf,buf_len,0,(SOCKADDR *)addr,(socklen_t *)addrlen);
	if (ret==SOCKET_ERROR)
	{
		int err=GETERROR;
		if (err!=WSAEWOULDBLOCK)
		{
			return -1;
		}
	}
	return ret;
}

void UDP_MAGR::udpThread(UDP_MAGR*p)
{
	int ret;
	char recvbuf[2000]={0};
	struct sockaddr_in addr;
	int addrlen=sizeof(struct sockaddr_in);	
	char ip[20]={0};
	unsigned short port;

	while(p->isRuning)
	{
		ret=p->blockingRecvfrom(p->_udpSocket,recvbuf,sizeof(recvbuf),&addr,&addrlen);
		if(ret>0)
		{
			zhSockAddrToPram(&addr,ip,&port);
			//printf("%s:%d->len=%d\n",ip,port,ret);
			p->setdelegate.callback_recv_cb(ip,port,recvbuf,ret);
		}
	}
}