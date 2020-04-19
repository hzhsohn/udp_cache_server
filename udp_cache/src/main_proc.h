#ifndef _ZH_PROCESS_BEGIN_H_
#include <stdio.h>
#include "session.h"
#include "udp_magr.h"
#include <vector>
using namespace std;

struct TagUserSrcData
{
		char flag_string[128]; //标记信息
		char hexbuf[1600];
		int hexlen;

		TagUserSrcData()
		{
			memset(flag_string,0,sizeof(flag_string));
			memset(hexbuf,0,sizeof(1600));
		}
};
struct TagUserDstData
{
		char ipv4[20];
		int port;
		char flag_string[128]; //标记信息
		time_t activeTime;//激活时间

		TagUserDstData()
		{
			memset(ipv4,0,sizeof(ipv4));
			port=0;
			memset(flag_string,0,sizeof(flag_string));
		}
};

class MainPROC
{
	UDP_MAGR _udpMagr;
	//服务器收到的转发数据
	vector<TagUserSrcData> lstUSrcData;
	vector<TagUserSrcData>::iterator itUSrc;
	TYPE_CS csUSrcData;
	vector<TagUserDstData> lstUDstData;
	vector<TagUserDstData>::iterator itUDst;
	TYPE_CS csUDstData;

public:
	
	bool InitProc(int argc,char *argv[]);
	bool RunProc();
	void EndProc();

public:

	void udp_recvf(char*ip,int port,char* data,int len);
};

#define _ZH_PROCESS_BEGIN_H_
#endif
