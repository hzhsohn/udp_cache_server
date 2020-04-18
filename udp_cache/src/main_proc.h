#ifndef _ZH_PROCESS_BEGIN_H_
#include <stdio.h>
#include "session.h"
#include "udp_magr.h"

class MainPROC
{
	UDP_MAGR	_udpMagr;

public:
	
	bool InitProc(int argc,char *argv[]);
	bool RunProc();
	void EndProc();

public:

	void udp_recvf(char*ip,int port,char* data,int len);
};

#define _ZH_PROCESS_BEGIN_H_
#endif
