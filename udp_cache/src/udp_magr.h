#ifndef _ZH_PROCESS_UDP_PROC_H_
#include <stdio.h>
#include "session.h"
#include "udp_magr_delegate.h"
#include "micro.h"

class MainPROC;
class UDP_MAGR
{
public:
	UDP_MAGR();
	~UDP_MAGR();

public:
	
	BOOL isRuning;
	SOCKET _udpSocket;
	UDPMagrDelegate<MainPROC> setdelegate;

public:

	BOOL init(int port);
	void destory();

	int blockingRecvfrom(SOCKET s,char *buf, int buf_len, struct sockaddr_in *addr ,int *addrlen);
	static void udpThread(PVOID pp);

	int sendto(char* ipv4, int port ,char* buf,int len);
};

#define _ZH_PROCESS_UDP_PROC_H_
#endif
